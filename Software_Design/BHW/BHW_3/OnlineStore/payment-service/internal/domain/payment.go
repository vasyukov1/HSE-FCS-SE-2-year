package domain

import (
	"context"
	"errors"
	"github.com/google/uuid"
	"time"
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

type InboxMessage struct {
	ID          uuid.UUID  `json:"id"`
	ReceivedAt  time.Time  `json:"received_at"`
	EventType   string     `json:"event_type"`
	Payload     []byte     `json:"payload"`
	ProcessedAt *time.Time `json:"processed_at"`
}

type OutboxMessage struct {
	ID           uuid.UUID  `json:"id"`
	OccurredAt   time.Time  `json:"occurred_at"`
	EventType    string     `json:"event_type"`
	Payload      []byte     `json:"payload"`
	DispatchedAt *time.Time `json:"dispatched_at"`
}

type OrderCreatedEvent struct {
	OrderID     string  `json:"order_id"`
	UserID      string  `json:"user_id"`
	Amount      float64 `json:"amount"`
	Description string  `json:"description"`
	CreatedAt   string  `json:"created_at"`
}

type PaymentSucceededEvent struct {
	OrderID string  `json:"order_id"`
	Amount  float64 `json:"amount"`
}

type PaymentFailedEvent struct {
	OrderID string `json:"order_id"`
	Reason  string `json:"reason"`
}
