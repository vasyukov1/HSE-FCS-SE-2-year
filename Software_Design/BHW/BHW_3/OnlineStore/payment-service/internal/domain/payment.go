package domain

import (
	"context"
	"errors"
	"github.com/google/uuid"
)

type CreateAccountRequest struct{}

type TopUpRequest struct {
	AccountID uuid.UUID `json:"account_id"`
	Amount    float64   `json:"amount"`
}

type BalanceRequest struct {
	AccountID uuid.UUID `json:"account_id"`
}

type BalanceResponse struct {
	AccountID string  `json:"account_id"`
	Balance   float64 `json:"balance"`
}

type PaymentResponse struct {
	ContentType string
	Body        []byte
	StatusCode  int
}

type PaymentService interface {
	Create(ctx context.Context, req CreateAccountRequest) (uuid.UUID, error)
	TopUp(ctx context.Context, req TopUpRequest) (*BalanceResponse, error)
	Balance(ctx context.Context, req BalanceRequest) (*BalanceResponse, error)
	Withdraw(ctx context.Context, req WithdrawRequest) (*BalanceResponse, error)
}

type Storage interface {
	Save(ctx context.Context, id uuid.UUID) error
	Get(ctx context.Context, id uuid.UUID) (float64, error)
	UpdateBalance(ctx context.Context, id uuid.UUID, delta float64) (float64, error)
	Withdraw(ctx context.Context, id uuid.UUID, amount float64) (float64, error)
}

type WithdrawRequest struct {
	AccountID uuid.UUID `json:"account_id"`
	Amount    float64   `json:"amount"`
}

var (
	ErrInvalidAmount     = errors.New("amount must be greater than zero")
	ErrInsufficientFunds = errors.New("insufficient funds")
)
