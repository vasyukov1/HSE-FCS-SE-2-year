package animal

import (
	"errors"
	"time"
)

type Animal struct {
	id           AnimalID
	species      string
	name         string
	birthDate    time.Time
	gender       Gender
	food         FoodType
	healthStatus HealthStatus
	enclosureId  EnclosureID
}

func NewAnimal(id AnimalID, species, name string, birthDate time.Time, gender Gender, food FoodType) (*Animal, error) {
	if species == "" || name == "" || food == "" {
		return nil, errors.New("missing required fields")
	}
	return &Animal{
		id:           id,
		species:      species,
		name:         name,
		birthDate:    birthDate,
		gender:       gender,
		food:         food,
		healthStatus: Healthy,
	}, nil
}

func (a *Animal) Feed(food FoodType) bool {
	return a.food == food
}

func (a *Animal) Heal() {
	a.healthStatus = Healthy
}

func (a *Animal) MakeSick() {
	a.healthStatus = Sick
}

func (a *Animal) MoveToEnclosure(newEnclosureId EnclosureID) {
	a.enclosureId = newEnclosureId
}

func (a *Animal) ID() AnimalID               { return a.id }
func (a *Animal) Species() string            { return a.species }
func (a *Animal) Name() string               { return a.name }
func (a *Animal) BirthDate() time.Time       { return a.birthDate }
func (a *Animal) Gender() Gender             { return a.gender }
func (a *Animal) Food() FoodType             { return a.food }
func (a *Animal) HealthStatus() HealthStatus { return a.healthStatus }
func (a *Animal) EnclosureId() EnclosureID   { return a.enclosureId }
