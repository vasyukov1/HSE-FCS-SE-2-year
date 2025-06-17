package repository

import (
	"context"
	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"order-service/internal/domain"
)

type PgStorage struct {
	db *pgx.Conn
}

func NewPgStorage(db *pgx.Conn) *PgStorage {
	return &PgStorage{db}
}

func (s *PgStorage) Save(ctx context.Context, o *domain.Order) error {
	const sql = `
        INSERT INTO orders
            (id, user_id, amount, description, status, created_at, updated_at)
        VALUES
            ($1, $2, $3, $4, $5, $6, $7)
    `
	_, err := s.db.Exec(ctx, sql,
		o.ID, o.UserID, o.Amount, o.Description, o.Status,
		o.CreatedAt, o.UpdatedAt,
	)
	return err
}

func (s *PgStorage) GetAll(ctx context.Context) ([]domain.Order, error) {
	const sql = `
        SELECT id, user_id, amount, description, status, created_at, updated_at
        FROM orders
        ORDER BY created_at DESC
    `

	rows, err := s.db.Query(ctx, sql)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var list []domain.Order
	for rows.Next() {
		var o domain.Order
		if err := rows.Scan(
			&o.ID, &o.UserID, &o.Amount, &o.Description,
			&o.Status, &o.CreatedAt, &o.UpdatedAt,
		); err != nil {
			return nil, err
		}
		list = append(list, o)
	}
	return list, rows.Err()
}

func (s *PgStorage) GetByID(ctx context.Context, id uuid.UUID) (*domain.Order, error) {
	const sql = `
        SELECT id, user_id, amount, description, status, created_at, updated_at
          FROM orders
         WHERE id = $1
    `
	row := s.db.QueryRow(ctx, sql, id)
	var o domain.Order
	if err := row.Scan(
		&o.ID, &o.UserID, &o.Amount, &o.Description,
		&o.Status, &o.CreatedAt, &o.UpdatedAt,
	); err != nil {
		return nil, err
	}
	return &o, nil
}

func (s *PgStorage) UpdateStatus(ctx context.Context, o *domain.Order) error {
	const sql = `
		UPDATE orders
		SET status  = $1, updated_at = $2
		WHERE id = $3
	`
	_, err := s.db.Exec(ctx, sql, o.Status, o.UpdatedAt, o.ID)
	return err
}
