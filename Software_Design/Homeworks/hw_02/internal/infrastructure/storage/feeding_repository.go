package storage

import (
	"context"
	"errors"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"sync"
)

type FeedingRepository struct {
	mu      sync.RWMutex
	storage map[feeding.ScheduleID]*feeding.Feeding
}

func NewFeedingRepository() *FeedingRepository {
	return &FeedingRepository{
		storage: make(map[feeding.ScheduleID]*feeding.Feeding),
	}
}

func (repo *FeedingRepository) SaveFeeding(ctx context.Context, f *feeding.Feeding) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	repo.storage[f.ID()] = f
	return nil
}

func (repo *FeedingRepository) GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	feedings := make([]*feeding.Feeding, 0, len(repo.storage))
	for _, f := range repo.storage {
		feedings = append(feedings, f)
	}
	return feedings, nil
}

func (repo *FeedingRepository) GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	f, ok := repo.storage[id]
	if !ok {
		return nil, errors.New("feeding not found")
	}
	return f, nil
}

func (repo *FeedingRepository) GetFeedingByAnimalID(ctx context.Context, animalID animal.AnimalID) (*feeding.Feeding, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	for _, f := range repo.storage {
		if f.AnimalID() == string(animalID) {
			return f, nil
		}
	}
	return nil, errors.New("feeding not found")
}

func (repo *FeedingRepository) DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	if _, ok := repo.storage[id]; !ok {
		return errors.New("feeding not found")
	}

	delete(repo.storage, id)
	return nil
}
