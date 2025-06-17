package repository

import (
	"context"
	"errors"
	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	"payment-service/internal/domain"
)

type PgStorage struct {
	db *pgx.Conn
}

func NewPgStorage(db *pgx.Conn) *PgStorage {
	return &PgStorage{db}
}

var ErrAccountNotFound = errors.New("account not found")

func (s *PgStorage) Save(ctx context.Context, id uuid.UUID) error {
	baseAmount := 0.0
	_, err := s.db.Exec(
		ctx,
		"INSERT INTO account (id, balance) VALUES ($1, $2)",
		id, baseAmount,
	)
	return err
}

func (s *PgStorage) Get(ctx context.Context, id uuid.UUID) (float64, error) {
	var balance float64
	err := s.db.QueryRow(ctx, "SELECT balance FROM account WHERE id = $1", id).Scan(&balance)
	return balance, err
}

func (s *PgStorage) UpdateBalance(ctx context.Context, id uuid.UUID, delta float64) (float64, error) {
	var newBalance float64
	err := s.db.QueryRow(
		ctx,
		`UPDATE account
		     SET balance = balance + $2, updated_at = NOW()
		   WHERE id = $1
		   RETURNING balance`,
		id, delta,
	).Scan(&newBalance)

	if errors.Is(err, pgx.ErrNoRows) {
		return 0, ErrAccountNotFound
	}
	return newBalance, err
}

func (s *PgStorage) Withdraw(ctx context.Context, id uuid.UUID, amount float64) (float64, error) {
	var newBalance float64
	err := s.db.QueryRow(
		ctx,
		`UPDATE account
             SET balance = balance - $2, updated_at = NOW()
           WHERE id = $1 AND balance >= $2
           RETURNING balance`,
		id, amount,
	).Scan(&newBalance)

	if errors.Is(err, pgx.ErrNoRows) {
		bal, getErr := s.Get(ctx, id)
		if getErr != nil {
			return 0, ErrAccountNotFound
		}
		if bal < amount {
			return bal, domain.ErrInsufficientFunds
		}
		return 0, ErrAccountNotFound
	}
	return newBalance, err
}
