package storage

import (
	"context"
	"errors"
	"hw_02/internal/domain/animal"
	"sync"
)

type InMemoryAnimalRepository struct {
	mu      sync.RWMutex
	storage map[animal.AnimalID]*animal.Animal
}

func NewInMemoryAnimalRepository() *InMemoryAnimalRepository {
	return &InMemoryAnimalRepository{
		storage: make(map[animal.AnimalID]*animal.Animal),
	}
}

func (repo *InMemoryAnimalRepository) SaveAnimal(ctx context.Context, a *animal.Animal) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	repo.storage[a.ID()] = a
	return nil
}

func (repo *InMemoryAnimalRepository) GetAllAnimals(ctx context.Context) ([]*animal.Animal, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	animals := make([]*animal.Animal, 0, len(repo.storage))
	for _, a := range repo.storage {
		animals = append(animals, a)
	}
	return animals, nil
}

func (repo *InMemoryAnimalRepository) GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error) {
	repo.mu.RLock()
	defer repo.mu.RUnlock()

	a, ok := repo.storage[id]
	if !ok {
		return nil, errors.New("animal not found")
	}
	return a, nil
}

func (repo *InMemoryAnimalRepository) DeleteAnimal(ctx context.Context, id animal.AnimalID) error {
	repo.mu.Lock()
	defer repo.mu.Unlock()

	if _, ok := repo.storage[id]; !ok {
		return errors.New("animal not found")
	}

	delete(repo.storage, id)
	return nil
}
