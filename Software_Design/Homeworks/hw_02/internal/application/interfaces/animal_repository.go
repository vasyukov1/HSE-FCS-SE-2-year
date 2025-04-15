package interfaces

import (
	"context"
	"hw_02/internal/domain/animal"
)

type AnimalRepository interface {
	SaveAnimal(ctx context.Context, a *animal.Animal) error
	GetAllAnimals(ctx context.Context) ([]*animal.Animal, error)
	GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error)
	DeleteAnimal(ctx context.Context, id animal.AnimalID) error
}
