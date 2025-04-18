package dto

import (
	"time"
)

type AnimalResponse struct {
	ID                string    `json:"id"`
	Species           string    `json:"species"`
	Name              string    `json:"name"`
	BirthDate         time.Time `json:"birth_date"`
	Gender            string    `json:"gender"`
	Food              string    `json:"food"`
	FeedingScheduleID string    `json:"feeding_schedule_id"`
	HealthStatus      string    `json:"health_status"`
	EnclosureID       string    `json:"enclosure_id"`
}
