package infrastructure

import (
	"context"
	"fmt"
	"order-service/internal/kafka"
	"order-service/internal/payment"
	"time"

	"github.com/google/uuid"
	"order-service/internal/domain"
)

type Server struct {
	storage       domain.Storage
	producer      *kafka.Producer
	paymentClient *payment.PaymentClient
}

func NewServer(storage domain.Storage, producer *kafka.Producer, paymentClient *payment.PaymentClient) *Server {
	return &Server{storage, producer, paymentClient}
}

func (s *Server) Create(ctx context.Context, req domain.CreateOrderRequest) (*domain.Order, error) {
	if req.Amount <= 0 {
		return nil, domain.ErrInvalidAmount
	}
	userID, err := uuid.Parse(req.UserID)
	if err != nil {
		return nil, domain.ErrInvalidUserID
	}

	now := time.Now().UTC()
	o := &domain.Order{
		ID:          uuid.New(),
		UserID:      userID,
		Amount:      req.Amount,
		Description: req.Description,
		Status:      domain.StatusNew,
		CreatedAt:   now,
		UpdatedAt:   now,
	}

	if err := s.storage.Save(ctx, o); err != nil {
		return nil, err
	}

	if s.paymentClient != nil {
		_, err := s.paymentClient.Withdraw(ctx, userID, req.Amount)
		if err != nil {
			o.Status = domain.StatusFailed
			o.UpdatedAt = time.Now().UTC()
			_ = s.storage.UpdateStatus(ctx, o)
			return nil, fmt.Errorf("payment failed: %w", err)
		}
		o.Status = domain.StatusFinished
		o.UpdatedAt = time.Now().UTC()
		_ = s.storage.UpdateStatus(ctx, o)
	}

	if s.producer != nil {
		event := domain.OrderCreatedEvent{
			OrderID:     o.ID.String(),
			UserID:      o.UserID.String(),
			Amount:      o.Amount,
			Description: o.Description,
			CreatedAt:   o.CreatedAt.Format(time.RFC3339),
		}
		_ = s.producer.SendOrderCreated(ctx, event)
	}

	return o, nil
}

func (s *Server) List(ctx context.Context) ([]domain.Order, error) {
	return s.storage.GetAll(ctx)
}

func (s *Server) GetByID(ctx context.Context, id uuid.UUID) (*domain.Order, error) {
	return s.storage.GetByID(ctx, id)
}

func (s *Server) UpdateOrderStatus(ctx context.Context, orderID uuid.UUID, status domain.OrderStatus) error {
	o, err := s.storage.GetByID(ctx, orderID)
	if err != nil {
		return err
	}
	o.Status = status
	o.UpdatedAt = time.Now().UTC()
	return s.storage.UpdateStatus(ctx, o)
}
