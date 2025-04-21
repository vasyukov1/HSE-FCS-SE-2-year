package services

import (
	"context"
	"errors"
	"fmt"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/events"
	"time"
)

type EnclosureService struct {
	enclosureInterface interfaces.EnclosureInterface
	animalInterface    interfaces.AnimalInterface
	eventDispatcher    events.EventDispatcher
}

func NewEnclosureService(
	enclosureInterface interfaces.EnclosureInterface,
	animalInterface interfaces.AnimalInterface,
	eventDispatcher events.EventDispatcher,
) *EnclosureService {
	return &EnclosureService{
		enclosureInterface: enclosureInterface,
		animalInterface:    animalInterface,
		eventDispatcher:    eventDispatcher,
	}
}

func (s *EnclosureService) SaveEnclosure(ctx context.Context, e *enclosure.Enclosure) error {
	return s.enclosureInterface.SaveEnclosure(ctx, e)
}

func (s *EnclosureService) GetAllEnclosure(ctx context.Context) ([]*enclosure.Enclosure, error) {
	return s.enclosureInterface.GetAllEnclosure(ctx)
}

func (s *EnclosureService) GetEnclosureByID(ctx context.Context, id enclosure.EnclosureID) (*enclosure.Enclosure, error) {
	return s.enclosureInterface.GetEnclosureByID(ctx, id)
}

func (s *EnclosureService) DeleteEnclosure(ctx context.Context, id enclosure.EnclosureID) error {
	return s.enclosureInterface.DeleteEnclosure(ctx, id)
}

func (s *EnclosureService) AddAnimalToEnclosure(ctx context.Context, enclosureID enclosure.EnclosureID, animalID animal.AnimalID) error {
	a, err := s.animalInterface.GetAnimalByID(ctx, animalID)
	if err != nil {
		return errors.New("animal not found")
	}

	e, err := s.enclosureInterface.GetEnclosureByID(ctx, enclosureID)
	if err != nil {
		return errors.New("enclosure not found")
	}

	oldE, err := s.enclosureInterface.GetEnclosureByID(ctx, enclosure.EnclosureID(a.EnclosureId()))

	if err != nil {
		if err.Error() != "enclosure not found" {
			return fmt.Errorf("error with enclosure: %w", err)
		}
	} else {
		if err := oldE.RemoveAnimalFromEnclosure(animalID); err != nil {
			return fmt.Errorf("cannot remove animal from enclosure: %w", err)
		}
	}

	if len(e.GetAnimals()) == e.Capacity() {
		return errors.New("enclosure is full")
	}

	if err := e.AddAnimalToEnclosure(animalID); err != nil {
		return errors.New("cannot add animal to enclosure")
	}

	a.MoveToEnclosure(string(enclosureID))

	oldEID := enclosure.EnclosureID("")
	if oldE != nil {
		oldEID = oldE.ID()
	}
	event := events.AnimalMovedEvent{
		AnimalID:     animalID,
		OldEnclosure: oldEID,
		NewEnclosure: e.ID(),
		MovedAt:      time.Now(),
	}

	s.eventDispatcher.Dispatch(event)

	if err := s.animalInterface.SaveAnimal(ctx, a); err != nil {
		return errors.New("failed to save animal")
	}
	if err := s.enclosureInterface.SaveEnclosure(ctx, e); err != nil {
		return errors.New("failed to save enclosure")
	}

	return nil
}

func (s *EnclosureService) RemoveAnimalFromEnclosure(ctx context.Context, enclosureID enclosure.EnclosureID, animalID animal.AnimalID) error {
	a, err := s.animalInterface.GetAnimalByID(ctx, animalID)
	if err != nil {
		return errors.New("animal not found")
	}

	a.RemoveFromEnclosure()

	e, err := s.enclosureInterface.GetEnclosureByID(ctx, enclosureID)
	if err != nil {
		return errors.New("enclosure not found")
	}

	if err := e.RemoveAnimalFromEnclosure(animalID); err != nil {
		return errors.New("cannot remove animal from enclosure")
	}

	if err := s.animalInterface.SaveAnimal(ctx, a); err != nil {
		return errors.New("failed to save animal")
	}
	if err := s.enclosureInterface.SaveEnclosure(ctx, e); err != nil {
		return errors.New("failed to save enclosure")
	}

	return nil
}
