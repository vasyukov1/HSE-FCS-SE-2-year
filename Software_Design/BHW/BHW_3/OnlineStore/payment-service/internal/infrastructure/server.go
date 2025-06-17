package infrastructure

import (
	"context"
	"github.com/google/uuid"
	"payment-service/internal/domain"
)

type Server struct {
	storage domain.Storage
}

func NewServer(storage domain.Storage) *Server {
	return &Server{storage}
}

func (s *Server) Create(ctx context.Context, _ domain.CreateAccountRequest) (uuid.UUID, error) {
	id := uuid.New()
	if err := s.storage.Save(ctx, id); err != nil {
		return uuid.Nil, err
	}
	return id, nil
}

func (s *Server) TopUp(ctx context.Context, req domain.TopUpRequest) (*domain.BalanceResponse, error) {
	if req.Amount <= 0 {
		return nil, domain.ErrInvalidAmount
	}

	newBal, err := s.storage.UpdateBalance(ctx, req.AccountID, req.Amount)
	if err != nil {
		return nil, err
	}

	return &domain.BalanceResponse{
		AccountID: req.AccountID.String(),
		Balance:   newBal,
	}, nil
}

func (s *Server) Balance(ctx context.Context, req domain.BalanceRequest) (*domain.BalanceResponse, error) {
	balance, err := s.storage.Get(ctx, req.AccountID)
	if err != nil {
		return nil, err
	}

	return &domain.BalanceResponse{
		AccountID: req.AccountID.String(),
		Balance:   balance,
	}, nil
}

func (s *Server) Withdraw(ctx context.Context, req domain.WithdrawRequest) (*domain.BalanceResponse, error) {
	if req.Amount <= 0 {
		return nil, domain.ErrInvalidAmount
	}
	newBal, err := s.storage.Withdraw(ctx, req.AccountID, req.Amount)
	if err != nil {
		return nil, err
	}
	return &domain.BalanceResponse{
		AccountID: req.AccountID.String(),
		Balance:   newBal,
	}, nil
}
