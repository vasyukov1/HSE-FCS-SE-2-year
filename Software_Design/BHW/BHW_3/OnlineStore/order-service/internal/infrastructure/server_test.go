package infrastructure

import (
	"context"
	"errors"
	"github.com/google/uuid"
	"order-service/internal/domain"
	"testing"
	"time"
)

type testStorage struct {
	saveCalled         bool
	updateStatusCalled bool
}

func (s *testStorage) Save(ctx context.Context, o *domain.Order) error {
	s.saveCalled = true
	return nil
}
func (s *testStorage) GetAll(ctx context.Context) ([]domain.Order, error) { return nil, nil }
func (s *testStorage) GetByID(ctx context.Context, id uuid.UUID) (*domain.Order, error) {
	return nil, nil
}
func (s *testStorage) UpdateStatus(ctx context.Context, o *domain.Order) error {
	s.updateStatusCalled = true
	return nil
}

func TestServer_Create_Success(t *testing.T) {
	storage := &testStorage{}
	server := NewServer(storage, nil, nil)

	req := domain.CreateOrderRequest{
		UserID:      uuid.New().String(),
		Amount:      100,
		Description: "test",
	}
	order, err := server.Create(context.Background(), req)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if order == nil || order.Amount != 100 || order.Description != "test" {
		t.Fatalf("unexpected order: %+v", order)
	}
	if !storage.saveCalled {
		t.Error("Save was not called")
	}
}

func TestServer_Create_InvalidAmount(t *testing.T) {
	server := NewServer(&testStorage{}, nil, nil)
	req := domain.CreateOrderRequest{
		UserID:      uuid.New().String(),
		Amount:      0,
		Description: "test",
	}
	_, err := server.Create(context.Background(), req)
	if err != domain.ErrInvalidAmount {
		t.Fatalf("expected ErrInvalidAmount, got %v", err)
	}
}

func TestServer_Create_InvalidUserID(t *testing.T) {
	server := NewServer(&testStorage{}, nil, nil)
	req := domain.CreateOrderRequest{
		UserID:      "not-a-uuid",
		Amount:      10,
		Description: "test",
	}
	_, err := server.Create(context.Background(), req)
	if err != domain.ErrInvalidUserID {
		t.Fatalf("expected ErrInvalidUserID, got %v", err)
	}
}

type failStorage struct{ testStorage }

func (s *failStorage) Save(ctx context.Context, o *domain.Order) error {
	return context.DeadlineExceeded
}

func TestServer_Create_SaveError(t *testing.T) {
	storage := &failStorage{}
	server := NewServer(storage, nil, nil)
	req := domain.CreateOrderRequest{
		UserID:      uuid.New().String(),
		Amount:      10,
		Description: "test",
	}
	_, err := server.Create(context.Background(), req)
	if err != context.DeadlineExceeded {
		t.Fatalf("expected context.DeadlineExceeded, got %v", err)
	}
}

func (s *stubStorage) Save(ctx context.Context, o *domain.Order) error {
	panic("")
}
func (s *stubStorage) GetAll(ctx context.Context) ([]domain.Order, error) {
	panic("")
}
func (s *stubStorage) GetByID(ctx context.Context, id uuid.UUID) (*domain.Order, error) {
	s.getByID = id
	if s.getByIDErr != nil {
		return nil, s.getByIDErr
	}
	return &domain.Order{
		ID:        id,
		Status:    domain.StatusNew,
		CreatedAt: time.Date(2000, 1, 1, 0, 0, 0, 0, time.UTC),
		UpdatedAt: time.Date(2000, 1, 1, 0, 0, 0, 0, time.UTC),
	}, nil
}
func (s *stubStorage) UpdateStatus(ctx context.Context, o *domain.Order) error {
	s.updateStatusArg = o
	return s.updateStatusErr
}

func TestServer_UpdateOrderStatus_Success(t *testing.T) {
	ctx := context.Background()
	orderID := uuid.New()

	stub := &stubStorage{}
	srv := NewServer(stub, nil, nil)

	err := srv.UpdateOrderStatus(ctx, orderID, domain.StatusFinished)
	if err != nil {
		t.Fatalf("ожидали no error, получили %v", err)
	}

	if stub.getByID != orderID {
		t.Errorf("GetByID был вызван с %v, ожидали %v", stub.getByID, orderID)
	}

	if stub.updateStatusArg == nil {
		t.Fatal("UpdateStatus не был вызван")
	}
	if stub.updateStatusArg.Status != domain.StatusFinished {
		t.Errorf("Order.Status = %q, ожидали %q", stub.updateStatusArg.Status, domain.StatusFinished)
	}
	if !stub.updateStatusArg.UpdatedAt.After(stub.updateStatusArg.CreatedAt) {
		t.Errorf("UpdatedAt (%v) не позже CreatedAt (%v)",
			stub.updateStatusArg.UpdatedAt, stub.updateStatusArg.CreatedAt)
	}
}

func TestServer_UpdateOrderStatus_GetByIDError(t *testing.T) {
	ctx := context.Background()
	orderID := uuid.New()

	stubErr := errors.New("db down")
	stub := &stubStorage{getByIDErr: stubErr}
	srv := NewServer(stub, nil, nil)

	err := srv.UpdateOrderStatus(ctx, orderID, domain.StatusFinished)
	if !errors.Is(err, stubErr) {
		t.Fatalf("ожидали ошибку %v, получили %v", stubErr, err)
	}

	if stub.updateStatusArg != nil {
		t.Errorf("UpdateStatus не должен был вызываться, но получил %+v", stub.updateStatusArg)
	}
}

type stubStorage struct {
	getByID         uuid.UUID
	updateStatusArg *domain.Order

	getByIDErr      error
	updateStatusErr error
}

func TestServer_UpdateOrderStatus_UpdateStatusError(t *testing.T) {
	ctx := context.Background()
	orderID := uuid.New()

	stubErr := errors.New("cannot update")
	stub := &stubStorage{updateStatusErr: stubErr}
	srv := NewServer(stub, nil, nil)

	err := srv.UpdateOrderStatus(ctx, orderID, domain.StatusFailed)
	if !errors.Is(err, stubErr) {
		t.Fatalf("ожидали ошибку %v, получили %v", stubErr, err)
	}

	if stub.getByID != orderID {
		t.Errorf("GetByID был вызван с %v, ожидали %v", stub.getByID, orderID)
	}
	if stub.updateStatusArg == nil {
		t.Fatal("ожидали, что UpdateStatus будет вызван, но он не вызывался")
	}
}
