package services

import (
	"context"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
)

type AnimalService struct {
	animalInterface interfaces.AnimalInterface
}

func NewAnimalService(animalInterface interfaces.AnimalInterface) *AnimalService {
	return &AnimalService{animalInterface: animalInterface}
}

func (s *AnimalService) SaveAnimal(ctx context.Context, a *animal.Animal) error {
	return s.animalInterface.SaveAnimal(ctx, a)
}

func (s *AnimalService) GetAllAnimals(ctx context.Context) ([]*animal.Animal, error) {
	return s.animalInterface.GetAllAnimals(ctx)
}

func (s *AnimalService) GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error) {
	return s.animalInterface.GetAnimalByID(ctx, id)
}

func (s *AnimalService) DeleteAnimal(ctx context.Context, id animal.AnimalID) error {
	return s.animalInterface.DeleteAnimal(ctx, id)
}
