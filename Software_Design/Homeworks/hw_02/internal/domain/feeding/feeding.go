package feeding

import (
	"errors"
)

type Feeding struct {
	id       ScheduleID
	animalID string
	time     string
	food     FoodType
}

func NewFeeding(id ScheduleID, animalID string, time string, food FoodType) (*Feeding, error) {
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

func (f *Feeding) EditTime(newTime string) {
	f.time = newTime
}

func (f *Feeding) EditFood(newFood FoodType) {
	f.food = newFood
}

func (f *Feeding) ID() ScheduleID   { return f.id }
func (f *Feeding) AnimalID() string { return f.animalID }
func (f *Feeding) Time() string     { return f.time }
func (f *Feeding) Food() FoodType   { return f.food }
