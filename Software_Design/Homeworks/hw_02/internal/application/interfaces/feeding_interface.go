package interfaces

import (
	"context"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
)

type FeedingInterface interface {
	SaveFeeding(ctx context.Context, f *feeding.Feeding) error
	GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error)
	GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error)
	GetFeedingByAnimalID(ctx context.Context, id animal.AnimalID) (*feeding.Feeding, error)
	DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error
}
