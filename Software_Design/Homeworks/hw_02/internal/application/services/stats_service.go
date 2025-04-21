package services

import (
	"context"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/feeding"
)

type StatsService struct {
	animalInterface    interfaces.AnimalInterface
	enclosureInterface interfaces.EnclosureInterface
	feedingInterface   interfaces.FeedingInterface
}

func NewStatsService(
	animalInterface interfaces.AnimalInterface,
	enclosureInterface interfaces.EnclosureInterface,
	feedingInterface interfaces.FeedingInterface,
) *StatsService {
	return &StatsService{
		animalInterface:    animalInterface,
		enclosureInterface: enclosureInterface,
		feedingInterface:   feedingInterface,
	}
}

func (s *StatsService) GetAnimals(ctx context.Context) []*animal.Animal {
	animals, err := s.animalInterface.GetAllAnimals(ctx)
	if err != nil {
		return []*animal.Animal{}
	}
	return animals
}

func (s *StatsService) GetEnclosures(ctx context.Context) []*enclosure.Enclosure {
	enclosures, err := s.enclosureInterface.GetAllEnclosure(ctx)
	if err != nil {
		return []*enclosure.Enclosure{}
	}
	return enclosures
}

func (s *StatsService) GetFeedings(ctx context.Context) []*feeding.Feeding {
	feedings, err := s.feedingInterface.GetAllFeedings(ctx)
	if err != nil {
		return []*feeding.Feeding{}
	}
	return feedings
}
