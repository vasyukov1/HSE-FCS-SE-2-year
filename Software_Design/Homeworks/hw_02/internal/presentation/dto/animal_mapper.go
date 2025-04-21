package dto

import (
	"hw_02/internal/domain/animal"
)

func MapAnimalToResponse(a *animal.Animal) AnimalResponse {
	return AnimalResponse{
		ID:                string(a.ID()),
		Species:           a.Species(),
		Name:              a.Name(),
		BirthDate:         a.BirthDate(),
		Gender:            string(a.Gender()),
		Feed:              a.Food(),
		FeedingScheduleID: a.FeedingScheduleID(),
		HealthStatus:      string(a.HealthStatus()),
		EnclosureID:       a.EnclosureId(),
	}
}

func MapAnimalsToResponseList(animals []*animal.Animal) []AnimalResponse {
	result := make([]AnimalResponse, 0, len(animals))
	for _, a := range animals {
		result = append(result, MapAnimalToResponse(a))
	}
	return result
}
