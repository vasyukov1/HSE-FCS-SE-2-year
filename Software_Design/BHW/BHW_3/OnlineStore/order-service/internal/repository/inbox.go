package repository

import (
	"context"
	"time"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"order-service/internal/domain"
)

type InboxRepo struct {
	conn *pgx.Conn
}

func NewInboxRepo(conn *pgx.Conn) *InboxRepo {
	return &InboxRepo{conn}
}

func (r *InboxRepo) SaveIfNotExists(ctx context.Context, msg *domain.InboxMessage) error {
	const sql = `
        INSERT INTO inbox (id, received_at, event_type, payload)
        VALUES ($1, $2, $3, $4)
      ON CONFLICT (id) DO NOTHING
    `
	_, err := r.conn.Exec(ctx, sql,
		msg.ID, msg.ReceivedAt, msg.EventType, msg.Payload,
	)
	return err
}

func (r *InboxRepo) Get(ctx context.Context, id uuid.UUID) (*domain.InboxMessage, error) {
	const sql = `
        SELECT id, received_at, event_type, payload, processed_at
          FROM inbox
         WHERE id = $1
    `
	row := r.conn.QueryRow(ctx, sql, id)
	var m domain.InboxMessage
	if err := row.Scan(&m.ID, &m.ReceivedAt, &m.EventType, &m.Payload, &m.ProcessedAt); err != nil {
		return nil, err
	}
	return &m, nil
}

func (r *InboxRepo) MarkProcessed(ctx context.Context, id uuid.UUID) error {
	const sql = `
        UPDATE inbox
           SET processed_at = $1
         WHERE id = $2
    `
	_, err := r.conn.Exec(ctx, sql, time.Now().UTC(), id)
	return err
}
