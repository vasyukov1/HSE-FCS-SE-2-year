package domain

import (
	"context"
	"github.com/google/uuid"
)

type Analysis struct {
	ID             uuid.UUID
	Location       string
	WordCount      int
	ParagraphCount int
	CharacterCount int
}

type AnalyzeRequest struct {
	ID string `json:"id"`
}

type Storage interface {
	Save(ctx Context, a Analysis) error
	Get(ctx Context, id uuid.UUID) (Analysis, error)
}

type FileClient interface {
	FetchFile(ctx Context, id uuid.UUID) ([]byte, error)
}

type WordCloudClient interface {
	Generate(ctx Context, text []byte) ([]byte, error)
}

type ImageSaver interface {
	Save(id string, data []byte) (string, error)
}

type Context = context.Context
