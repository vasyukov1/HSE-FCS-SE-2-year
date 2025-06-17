package domain

import "context"

type TopUpRequest struct {
	AccountID string  `json:"account_id"`
	Amount    float64 `json:"amount"`
}

type BalanceRequest struct {
	AccountID string `json:"account_id"`
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
	CreateAccount(ctx context.Context) (*PaymentResponse, error)
	Balance(ctx context.Context, req BalanceRequest) (*BalanceResponse, error)
	TopUp(ctx context.Context, req TopUpRequest) (*PaymentResponse, error)
}
