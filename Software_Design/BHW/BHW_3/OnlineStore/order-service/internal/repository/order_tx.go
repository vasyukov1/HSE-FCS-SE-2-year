package repository

import (
	"context"
	"encoding/json"
	"time"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"order-service/internal/domain"
)

func SaveOrderWithOutbox(ctx context.Context, conn *pgx.Conn, o *domain.Order, event domain.OrderCreatedEvent) error {
	tx, err := conn.Begin(ctx)
	if err != nil {
		return err
	}
	defer tx.Rollback(ctx)

	const sqlOrder = `
        INSERT INTO orders (id, user_id, amount, description, status, created_at, updated_at)
        VALUES ($1,$2,$3,$4,$5,$6,$7)
    `
	if _, err := tx.Exec(ctx, sqlOrder,
		o.ID, o.UserID, o.Amount, o.Description, o.Status, o.CreatedAt, o.UpdatedAt,
	); err != nil {
		return err
	}

	payload, _ := json.Marshal(event)
	outMsg := &domain.OutboxMessage{
		ID:         uuid.New(),
		OccurredAt: time.Now().UTC(),
		EventType:  "OrderCreated",
		Payload:    payload,
	}
	outRepo := NewOutboxRepo(conn)
	if err := outRepo.Save(ctx, tx, outMsg); err != nil {
		return err
	}

	return tx.Commit(ctx)
}
