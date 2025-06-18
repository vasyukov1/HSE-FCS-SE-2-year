package repository

import (
	"context"
	"time"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"payment-service/internal/domain"
)

type OutboxRepo struct {
	conn *pgx.Conn
}

func NewOutboxRepo(conn *pgx.Conn) *OutboxRepo {
	return &OutboxRepo{conn}
}

func (r *OutboxRepo) Save(ctx context.Context, tx pgx.Tx, msg *domain.OutboxMessage) error {
	const sql = `
        INSERT INTO outbox (id, occurred_at, event_type, payload)
        VALUES ($1, $2, $3, $4)
    `
	_, err := tx.Exec(ctx, sql,
		msg.ID, msg.OccurredAt, msg.EventType, msg.Payload,
	)
	return err
}

func (r *OutboxRepo) FetchUnsent(ctx context.Context, limit int) ([]domain.OutboxMessage, error) {
	const sql = `
        SELECT id, occurred_at, event_type, payload
          FROM outbox
         WHERE dispatched_at IS NULL
      ORDER BY occurred_at
         LIMIT $1
    `
	rows, err := r.conn.Query(ctx, sql, limit)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var msgs []domain.OutboxMessage
	for rows.Next() {
		var m domain.OutboxMessage
		if err := rows.Scan(&m.ID, &m.OccurredAt, &m.EventType, &m.Payload); err != nil {
			return nil, err
		}
		msgs = append(msgs, m)
	}
	return msgs, rows.Err()
}

func (r *OutboxRepo) MarkDispatched(ctx context.Context, id uuid.UUID) error {
	const sql = `
        UPDATE outbox SET dispatched_at = $1 WHERE id = $2
    `
	_, err := r.conn.Exec(ctx, sql, time.Now().UTC(), id)
	return err
}
