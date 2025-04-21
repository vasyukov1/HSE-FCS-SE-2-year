package services

import (
	"context"
	"errors"
	"fmt"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
)

type AnimalService struct {
	animalInterface  interfaces.AnimalInterface
	feedingInterface interfaces.FeedingInterface
}

func NewAnimalService(animalInterface interfaces.AnimalInterface, feedingInterface interfaces.FeedingInterface) *AnimalService {
	return &AnimalService{
		animalInterface:  animalInterface,
		feedingInterface: feedingInterface,
	}
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

func (s *AnimalService) MakeSick(ctx context.Context, id animal.AnimalID) error {
	a, err := s.GetAnimalByID(ctx, id)
	if err != nil {
		return err
	}
	a.MakeSick()
	return nil
}

func (s *AnimalService) Heal(ctx context.Context, id animal.AnimalID) error {
	a, err := s.GetAnimalByID(ctx, id)
	if err != nil {
		return err
	}
	a.Heal()
	return nil
}

func (s *AnimalService) Feed(ctx context.Context, id animal.AnimalID, food string, feedingTime string) error {
	a, err := s.GetAnimalByID(ctx, id)
	if err != nil {
		return err
	}

	feedingScheduleID := feeding.ScheduleID(a.FeedingScheduleID())
	feedingSchedule, err := s.feedingInterface.GetFeedingByID(ctx, feedingScheduleID)
	if err != nil {
		return errors.New("could not get feeding schedule")
	}

	if feedingSchedule.Time() != feedingTime {
		return fmt.Errorf("time does not match, need to give %s", feedingSchedule.Time())
	}
	if string(feedingSchedule.Food()) != food {
		return fmt.Errorf("food does not match, need to give %s", feedingSchedule.Food())
	}

	a.Feed()
	return nil
}

func (s *AnimalService) Hungry(ctx context.Context, id animal.AnimalID) error {
	a, err := s.GetAnimalByID(ctx, id)
	if err != nil {
		return err
	}

	a.Hungry()
	return nil
}
