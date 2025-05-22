package domain

import (
	"context"
	"io"
)

type File struct {
	ID      string
	Name    string
	Content io.ReadCloser
}

type GetByIDResponse struct {
	ContentType string
	Body        []byte
	StatusCode  int
}

type FileService interface {
	Store(ctx context.Context, file *File) (string, error)
	Get(ctx context.Context, id string) (*File, error)
}
