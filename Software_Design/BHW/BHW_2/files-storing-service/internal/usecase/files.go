package usecase

import (
	"context"
	"crypto/sha256"
	"encoding/hex"
	"files-storing-service/internal/domain"
	"github.com/google/uuid"
)

type Interactor struct {
	Storage   domain.Storage
	FileSaver domain.FileSaver
}

func NewIntercator(
	storage domain.Storage,
	fileSaver domain.FileSaver,
) *Interactor {
	return &Interactor{
		Storage:   storage,
		FileSaver: fileSaver,
	}
}

func (i *Interactor) Store(ctx context.Context, filename string, content []byte) (uuid.UUID, error) {
	// Compute hash and ID
	hash := sha256.Sum256(content)
	hashStr := hex.EncodeToString(hash[:])

	id := uuid.New()

	// Save file
	location, err := i.FileSaver.Save(id.String(), content)
	if err != nil {
		return uuid.Nil, err
	}

	// Create metadata
	meta := domain.File{
		ID:       id,
		Name:     filename,
		Hash:     hashStr,
		Location: location,
	}

	// Save file in DB
	if err = i.Storage.Save(ctx, meta); err != nil {
		return uuid.Nil, err
	}

	return id, nil
}

func (i *Interactor) Get(ctx context.Context, id uuid.UUID) ([]byte, error) {
	// Get location
	location, err := i.Storage.Get(ctx, id)
	if err != nil {
		return nil, err
	}

	// Get data
	data, err := i.FileSaver.Read(location)
	if err != nil {
		return nil, err
	}

	return data, nil
}
