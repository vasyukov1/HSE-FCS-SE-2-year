package storage

import (
	"context"
	"errors"
	"hw_02/internal/domain/enclosure"
	"sync"
)

type EnclosureRepository struct {
	mu      sync.RWMutex
	storage map[enclosure.EnclosureID]*enclosure.Enclosure
}

func NewEnclosureRepository() *EnclosureRepository {
	return &EnclosureRepository{
		storage: make(map[enclosure.EnclosureID]*enclosure.Enclosure),
	}
}

func (repo *EnclosureRepository) SaveEnclosure(ctx context.Context, e *enclosure.Enclosure) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	repo.storage[e.ID()] = e
	return nil
}

func (repo *EnclosureRepository) GetAllEnclosure(ctx context.Context) ([]*enclosure.Enclosure, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	enclosures := make([]*enclosure.Enclosure, 0, len(repo.storage))
	for _, e := range repo.storage {
		enclosures = append(enclosures, e)
	}
	return enclosures, nil
}

func (repo *EnclosureRepository) GetEnclosureByID(ctx context.Context, id enclosure.EnclosureID) (*enclosure.Enclosure, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	e, ok := repo.storage[id]
	if !ok {
		return nil, errors.New("enclosure not found")
	}
	return e, nil
}

func (repo *EnclosureRepository) DeleteEnclosure(ctx context.Context, id enclosure.EnclosureID) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	_, ok := repo.storage[id]
	if !ok {
		return errors.New("enclosure not found")
	}

	delete(repo.storage, id)
	return nil
}
