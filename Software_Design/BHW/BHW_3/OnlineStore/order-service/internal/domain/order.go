package domain

import (
	"context"
	"errors"
	"github.com/google/uuid"
	"time"
)

type OrderStatus string

const (
	StatusNew      OrderStatus = "new"
	StatusFinished OrderStatus = "finished"
	StatusFailed   OrderStatus = "cancelled"
)

type Order struct {
	ID          uuid.UUID   `json:"id"`
	UserID      uuid.UUID   `json:"user_id"`
	Amount      float64     `json:"amount"`
	Description string      `json:"description"`
	Status      OrderStatus `json:"status"`
	CreatedAt   time.Time   `json:"created_at"`
	UpdatedAt   time.Time   `json:"updated_at"`
}

type CreateOrderRequest struct {
	UserID      string  `json:"user_id"`
	Amount      float64 `json:"amount"`
	Description string  `json:"description"`
}

type OrderCreatedEvent struct {
	OrderID     string  `json:"order_id"`
	UserID      string  `json:"user_id"`
	Amount      float64 `json:"amount"`
	Description string  `json:"description"`
	CreatedAt   string  `json:"created_at"`
}

type TopUpRequest struct {
	AccountID uuid.UUID `json:"account_id"`
	Amount    float64   `json:"amount"`
	Reference string    `json:"reference"`
}

type BalanceResponse struct {
	AccountID string  `json:"account_id"`
	Balance   float64 `json:"balance"`
}

type OrderService interface {
	Create(ctx context.Context, req CreateOrderRequest) (*Order, error)
	List(ctx context.Context) ([]Order, error)
	GetByID(ctx context.Context, id uuid.UUID) (*Order, error)
}

type Storage interface {
	Save(ctx context.Context, o *Order) error
	GetAll(ctx context.Context) ([]Order, error)
	GetByID(ctx context.Context, id uuid.UUID) (*Order, error)
	UpdateStatus(ctx context.Context, o *Order) error
}

var (
	ErrInvalidAmount = errors.New("amount must be greater than zero")
	ErrInvalidUserID = errors.New("invalid user_id")
)
