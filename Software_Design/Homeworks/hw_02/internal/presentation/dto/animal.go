package dto

import (
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"time"
)

type AnimalRequest struct {
	ID                animal.AnimalID `json:"id"`
	Species           string          `json:"species"`
	Name              string          `json:"name"`
	BirthDate         time.Time       `json:"birth_date"`
	Gender            string          `json:"gender"`
	Feed              bool            `json:"feed"`
	FeedingScheduleID string          `json:"feeding_schedule_id"`
}

type AnimalResponse struct {
	ID                string    `json:"id"`
	Species           string    `json:"species"`
	Name              string    `json:"name"`
	BirthDate         time.Time `json:"birth_date"`
	Gender            string    `json:"gender"`
	Feed              bool      `json:"feed"`
	FeedingScheduleID string    `json:"feeding_schedule_id"`
	HealthStatus      string    `json:"health_status"`
	EnclosureID       string    `json:"enclosure_id"`
}

type FeedRequest struct {
	Food        feeding.FoodType `json:"food"`
	FeedingTime string           `json:"feeding_time"`
}
