package services

import (
	"context"
	"errors"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/events"
	"hw_02/internal/domain/feeding"
	"time"
)

type FeedingService struct {
	feedingInterface interfaces.FeedingInterface
	animalInterface  interfaces.AnimalInterface
	eventDispatcher  events.EventDispatcher
}

func NewFeedingService(
	feedingInterface interfaces.FeedingInterface,
	animalInterface interfaces.AnimalInterface,
	eventDispatcher events.EventDispatcher,
) *FeedingService {
	return &FeedingService{
		feedingInterface: feedingInterface,
		animalInterface:  animalInterface,
		eventDispatcher:  eventDispatcher,
	}
}

func (s *FeedingService) SaveFeeding(ctx context.Context, f *feeding.Feeding) error {
	a, err := s.animalInterface.GetAnimalByID(ctx, animal.AnimalID(f.AnimalID()))
	if err != nil {
		return errors.New("animal not found")
	}

	a.SetFeedingSchedule(string(f.ID()))

	if err := s.animalInterface.SaveAnimal(ctx, a); err != nil {
		return errors.New("failed to save animal")
	}
	if err := s.feedingInterface.SaveFeeding(ctx, f); err != nil {
		return errors.New("failed to save feeding")
	}

	s.eventDispatcher.Dispatch(events.FeedingTimeEvent{
		AnimalID:     a.ID(),
		ScheduleID:   f.ID(),
		ScheduledFor: time.Now(),
	})

	return nil
}

func (s *FeedingService) EditFeeding(ctx context.Context, id feeding.ScheduleID, newTime string, newFood feeding.FoodType) error {
	f, err := s.feedingInterface.GetFeedingByID(ctx, id)
	if err != nil {
		return errors.New("feeding not found")
	}

	f.EditTime(newTime)
	f.EditFood(newFood)

	if err := s.feedingInterface.SaveFeeding(ctx, f); err != nil {
		return errors.New("failed to save feeding")
	}

	return nil
}

func (s *FeedingService) GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error) {
	return s.feedingInterface.GetAllFeedings(ctx)
}

func (s *FeedingService) GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error) {
	return s.feedingInterface.GetFeedingByID(ctx, id)
}

func (s *FeedingService) GetFeedingByAnimalID(ctx context.Context, animalID animal.AnimalID) (*feeding.Feeding, error) {
	return s.feedingInterface.GetFeedingByAnimalID(ctx, animalID)
}

func (s *FeedingService) DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error {
	f, err := s.feedingInterface.GetFeedingByID(ctx, id)
	if err != nil {
		return errors.New("feeding not found")
	}

	a, err := s.animalInterface.GetAnimalByID(ctx, animal.AnimalID(f.AnimalID()))
	if err != nil {
		return errors.New("animal not found")
	}

	a.RemoveFeedingSchedule()
	if err := s.animalInterface.SaveAnimal(ctx, a); err != nil {
		return errors.New("failed to save animal")
	}

	return s.feedingInterface.DeleteFeeding(ctx, id)
}
