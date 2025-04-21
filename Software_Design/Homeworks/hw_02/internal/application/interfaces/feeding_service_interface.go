package interfaces

import (
	"context"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
)

type FeedingServiceInterface interface {
	SaveFeeding(ctx context.Context, f *feeding.Feeding) error
	EditFeeding(ctx context.Context, id feeding.ScheduleID, newTime string, newFood feeding.FoodType) error
	DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error
	GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error)
	GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error)
	GetFeedingByAnimalID(ctx context.Context, animalID animal.AnimalID) (*feeding.Feeding, error)
}
