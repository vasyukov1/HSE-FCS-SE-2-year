package services

import (
	"context"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
)

type AnimalService struct {
	repo interfaces.AnimalRepository
}

func NewAnimalService(repo interfaces.AnimalRepository) *AnimalService {
	return &AnimalService{repo: repo}
}

func (s *AnimalService) SaveAnimal(ctx context.Context, a *animal.Animal) error {
	return s.repo.SaveAnimal(ctx, a)
}

func (s *AnimalService) GetAllAnimals(ctx context.Context) ([]*animal.Animal, error) {
	return s.repo.GetAllAnimals(ctx)
}

func (s *AnimalService) GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error) {
	return s.repo.GetAnimalByID(ctx, id)
}

func (s *AnimalService) DeleteAnimal(ctx context.Context, id animal.AnimalID) error {
	return s.repo.DeleteAnimal(ctx, id)
}
