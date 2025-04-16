package interfaces

import (
	"context"
	"hw_02/internal/domain/enclosure"
)

type EnclosureInterface interface {
	SaveEnclosure(ctx context.Context, e *enclosure.Enclosure) error
	GetAllEnclosure(ctx context.Context) ([]*enclosure.Enclosure, error)
	GetEnclosureByID(ctx context.Context, id enclosure.EnclosureID) (*enclosure.Enclosure, error)
	DeleteEnclosure(ctx context.Context, id enclosure.EnclosureID) error
}
