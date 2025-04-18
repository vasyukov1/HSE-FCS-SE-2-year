package feeding

import (
	"errors"
	"hw_02/internal/domain/animal"
)

type Feeding struct {
	id       ScheduleID
	animalID string
	time     string
	food     animal.FoodType
}

func NewFeeding(id ScheduleID, animalID string, time string, food animal.FoodType) (*Feeding, error) {
	if food == "" {
		return nil, errors.New("invalid food type")
	}
	return &Feeding{
		id:       id,
		animalID: animalID,
		time:     time,
		food:     food,
	}, nil
}

func (fs *Feeding) ID() ScheduleID        { return fs.id }
func (fs *Feeding) AnimalID() string      { return fs.animalID }
func (fs *Feeding) Time() string          { return fs.time }
func (fs *Feeding) Food() animal.FoodType { return fs.food }
