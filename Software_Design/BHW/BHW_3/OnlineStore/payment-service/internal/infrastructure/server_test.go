package infrastructure

import (
	"context"
	"errors"
	"testing"

	"github.com/google/uuid"
	"payment-service/internal/domain"
)

// --- Мок для domain.Storage ---
type mockStorage struct {
	saveErr         error
	updateBalResult float64
	updateBalErr    error
	getResult       float64
	getErr          error
	withdrawResult  float64
	withdrawErr     error
}

func (m *mockStorage) Save(ctx context.Context, id uuid.UUID) error {
	return m.saveErr
}
func (m *mockStorage) Get(ctx context.Context, id uuid.UUID) (float64, error) {
	return m.getResult, m.getErr
}
func (m *mockStorage) UpdateBalance(ctx context.Context, id uuid.UUID, delta float64) (float64, error) {
	return m.updateBalResult, m.updateBalErr
}
func (m *mockStorage) Withdraw(ctx context.Context, id uuid.UUID, amount float64) (float64, error) {
	return m.withdrawResult, m.withdrawErr
}

// --- Тесты ---

func TestServer_Create_Success(t *testing.T) {
	st := &mockStorage{}
	s := NewServer(st)
	id, err := s.Create(context.Background(), domain.CreateAccountRequest{})
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if id == uuid.Nil {
		t.Error("expected non-nil uuid")
	}
}

func TestServer_Create_SaveError(t *testing.T) {
	st := &mockStorage{saveErr: errors.New("fail")}
	s := NewServer(st)
	id, err := s.Create(context.Background(), domain.CreateAccountRequest{})
	if err == nil {
		t.Fatal("expected error, got nil")
	}
	if id != uuid.Nil {
		t.Error("expected nil uuid on error")
	}
}

func TestServer_TopUp_Success(t *testing.T) {
	st := &mockStorage{updateBalResult: 150}
	s := NewServer(st)
	req := domain.TopUpRequest{AccountID: uuid.New(), Amount: 50}
	resp, err := s.TopUp(context.Background(), req)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if resp.Balance != 150 {
		t.Errorf("expected balance 150, got %v", resp.Balance)
	}
}

func TestServer_TopUp_InvalidAmount(t *testing.T) {
	st := &mockStorage{}
	s := NewServer(st)
	req := domain.TopUpRequest{AccountID: uuid.New(), Amount: 0}
	_, err := s.TopUp(context.Background(), req)
	if err != domain.ErrInvalidAmount {
		t.Fatalf("expected ErrInvalidAmount, got %v", err)
	}
}

func TestServer_TopUp_UpdateError(t *testing.T) {
	st := &mockStorage{updateBalErr: errors.New("fail")}
	s := NewServer(st)
	req := domain.TopUpRequest{AccountID: uuid.New(), Amount: 10}
	_, err := s.TopUp(context.Background(), req)
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}

func TestServer_Balance_Success(t *testing.T) {
	st := &mockStorage{getResult: 200}
	s := NewServer(st)
	req := domain.BalanceRequest{AccountID: uuid.New()}
	resp, err := s.Balance(context.Background(), req)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if resp.Balance != 200 {
		t.Errorf("expected balance 200, got %v", resp.Balance)
	}
}

func TestServer_Balance_GetError(t *testing.T) {
	st := &mockStorage{getErr: errors.New("fail")}
	s := NewServer(st)
	req := domain.BalanceRequest{AccountID: uuid.New()}
	_, err := s.Balance(context.Background(), req)
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}

func TestServer_Withdraw_Success(t *testing.T) {
	st := &mockStorage{withdrawResult: 80}
	s := NewServer(st)
	req := domain.WithdrawRequest{AccountID: uuid.New(), Amount: 20}
	resp, err := s.Withdraw(context.Background(), req)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if resp.Balance != 80 {
		t.Errorf("expected balance 80, got %v", resp.Balance)
	}
}

func TestServer_Withdraw_InvalidAmount(t *testing.T) {
	st := &mockStorage{}
	s := NewServer(st)
	req := domain.WithdrawRequest{AccountID: uuid.New(), Amount: 0}
	_, err := s.Withdraw(context.Background(), req)
	if err != domain.ErrInvalidAmount {
		t.Fatalf("expected ErrInvalidAmount, got %v", err)
	}
}

func TestServer_Withdraw_Error(t *testing.T) {
	st := &mockStorage{withdrawErr: errors.New("fail")}
	s := NewServer(st)
	req := domain.WithdrawRequest{AccountID: uuid.New(), Amount: 10}
	_, err := s.Withdraw(context.Background(), req)
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}
