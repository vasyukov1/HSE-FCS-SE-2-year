package animal

import (
	"errors"
	"time"
)

type Animal struct {
	id                AnimalID
	species           string
	name              string
	birthDate         time.Time
	gender            Gender
	feed              bool
	feedingScheduleID string
	healthStatus      HealthStatus
	enclosureId       string
}

func NewAnimal(id AnimalID, species, name string, birthDate time.Time, gender Gender) (*Animal, error) {
	if species == "" || name == "" {
		return nil, errors.New("missing required fields")
	}
	return &Animal{
		id:           id,
		species:      species,
		name:         name,
		birthDate:    birthDate,
		gender:       gender,
		feed:         false,
		healthStatus: Healthy,
	}, nil
}

func (a *Animal) Feed() {
	a.feed = true
}

func (a *Animal) Hungry() {
	a.feed = false
}

func (a *Animal) Heal() {
	a.healthStatus = Healthy
}

func (a *Animal) MakeSick() {
	a.healthStatus = Sick
}

func (a *Animal) IsHealthy() bool {
	return a.healthStatus == Healthy
}

func (a *Animal) IsMale() bool {
	return a.gender == "Male"
}

func (a *Animal) MoveToEnclosure(enclosureId string) {
	a.enclosureId = enclosureId
}

func (a *Animal) RemoveFromEnclosure() {
	a.enclosureId = ""
}

func (a *Animal) SetFeedingSchedule(feedingScheduleID string) {
	a.feedingScheduleID = feedingScheduleID
}

func (a *Animal) RemoveFeedingSchedule() {
	a.feedingScheduleID = ""
}

func (a *Animal) ID() AnimalID               { return a.id }
func (a *Animal) Species() string            { return a.species }
func (a *Animal) Name() string               { return a.name }
func (a *Animal) BirthDate() time.Time       { return a.birthDate }
func (a *Animal) Gender() Gender             { return a.gender }
func (a *Animal) Food() bool                 { return a.feed }
func (a *Animal) FeedingScheduleID() string  { return a.feedingScheduleID }
func (a *Animal) HealthStatus() HealthStatus { return a.healthStatus }
func (a *Animal) EnclosureId() string        { return a.enclosureId }
