package interfaces

import (
	"context"
	"hw_02/internal/domain/animal"
)

type AnimalServiceInterface interface {
	SaveAnimal(ctx context.Context, a *animal.Animal) error
	GetAllAnimals(ctx context.Context) ([]*animal.Animal, error)
	GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error)
	DeleteAnimal(ctx context.Context, id animal.AnimalID) error
	MakeSick(ctx context.Context, id animal.AnimalID) error
	Heal(ctx context.Context, id animal.AnimalID) error
	Feed(ctx context.Context, id animal.AnimalID, food string, feedingTime string) error
	Hungry(ctx context.Context, id animal.AnimalID) error
}
