package domain

import (
	"context"
)

type OrderRequest struct {
	UserID      string  `json:"user_id"`
	Amount      float64 `json:"amount"`
	Description string  `json:"description"`
}

type OrderResponse struct {
	ID          string  `json:"id"`
	UserID      string  `json:"user_id"`
	Amount      float64 `json:"amount"`
	Description string  `json:"description"`
	Status      string  `json:"status"`
	CreatedAt   string  `json:"created_at"`
	UpdatedAt   string  `json:"updated_at"`
}

type OrderListResponse []OrderResponse

type StatusResponse struct {
	ID     string `json:"id"`
	Status string `json:"status"`
}

type OrderService interface {
	Create(ctx context.Context, req OrderRequest) (*OrderResponse, error)
	List(ctx context.Context) ([]OrderResponse, error)
	Status(ctx context.Context, orderID string) (*StatusResponse, error)
}
