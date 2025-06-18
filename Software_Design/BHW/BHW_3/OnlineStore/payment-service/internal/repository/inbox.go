package repository

import (
	"context"
	"time"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"payment-service/internal/domain"
)

type InboxRepo struct {
	conn *pgx.Conn
}

func NewInboxRepo(conn *pgx.Conn) *InboxRepo {
	return &InboxRepo{conn}
}

func (r *InboxRepo) SaveIfNotExists(ctx context.Context, tx pgx.Tx, msg *domain.InboxMessage) error {
	const sql = `
        INSERT INTO inbox (id, received_at, event_type, payload)
        VALUES ($1, $2, $3, $4)
      ON CONFLICT (id) DO NOTHING
    `
	_, err := tx.Exec(ctx, sql,
		msg.ID, msg.ReceivedAt, msg.EventType, msg.Payload,
	)
	return err
}

func (r *InboxRepo) MarkProcessed(ctx context.Context, id uuid.UUID) error {
	const sql = `
        UPDATE inbox SET processed_at = $1 WHERE id = $2
    `
	_, err := r.conn.Exec(ctx, sql, time.Now().UTC(), id)
	return err
}
