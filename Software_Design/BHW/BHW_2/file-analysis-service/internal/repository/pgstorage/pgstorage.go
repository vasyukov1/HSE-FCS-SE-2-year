package pgstorage

import (
	"context"
	"errors"
	"file-analysis-service/internal/domain"
	"fmt"
	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
)

type PgStorage struct {
	db *pgx.Conn
}

func NewPgStorage(db *pgx.Conn) *PgStorage {
	return &PgStorage{db: db}
}

func (s *PgStorage) Save(ctx context.Context, obj domain.Analysis) error {
	_, err := s.db.Exec(ctx,
		"INSERT INTO analysis (id, location, word_count, paragraph_count, character_count) VALUES ($1, $2, $3, $4, $5)",
		obj.ID, obj.Location,
		obj.WordCount, obj.ParagraphCount, obj.CharacterCount,
	)
	return err
}

func (s *PgStorage) Get(ctx context.Context, id uuid.UUID) (domain.Analysis, error) {
	var obj domain.Analysis
	err := s.db.QueryRow(ctx,
		`SELECT id, location, word_count, paragraph_count, character_count 
        FROM analysis 
        WHERE id = $1`,
		id,
	).Scan(
		&obj.ID,
		&obj.Location,
		&obj.WordCount,
		&obj.ParagraphCount,
		&obj.CharacterCount,
	)
	if err != nil {
		if errors.Is(err, pgx.ErrNoRows) {
			return domain.Analysis{}, fmt.Errorf("analysis not found")
		}
		return domain.Analysis{}, fmt.Errorf("failed to get analysis: %w", err)
	}
	return obj, nil
}
