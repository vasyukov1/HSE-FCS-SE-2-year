package infrastructure

import (
	"context"
	"github.com/google/uuid"
	"order-service/internal/domain"
	"testing"
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
