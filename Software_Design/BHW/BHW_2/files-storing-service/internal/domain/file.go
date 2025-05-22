package domain

import (
	"context"
	"github.com/google/uuid"
)

type File struct {
	ID       uuid.UUID
	Name     string
	Hash     string
	Location string
}

type Storage interface {
	Save(ctx context.Context, file File) error
	Get(ctx context.Context, id uuid.UUID) (string, error)
}

type FileSaver interface {
	Save(id string, data []byte) (string, error)
	Read(location string) ([]byte, error)
}

type Context = context.Context
