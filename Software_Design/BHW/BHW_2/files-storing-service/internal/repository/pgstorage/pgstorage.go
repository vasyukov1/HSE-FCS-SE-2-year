package pgstorage

import (
	"context"
	"files-storing-service/internal/domain"
	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
)

type PgStorage struct {
	db *pgx.Conn
}

func NewPgStorage(db *pgx.Conn) *PgStorage {
	return &PgStorage{db: db}
}

func (s *PgStorage) Save(ctx context.Context, file domain.File) error {
	_, err := s.db.Exec(ctx,
		"INSERT INTO files (id, name, hash, location) VALUES ($1, $2, $3, $4)",
		file.ID, file.Name, file.Hash, file.Location,
	)
	return err
}

func (s *PgStorage) Get(ctx context.Context, id uuid.UUID) (string, error) {
	var location string
	err := s.db.QueryRow(ctx, "SELECT location FROM files WHERE id = $1", id).Scan(&location)
	return location, err
}
