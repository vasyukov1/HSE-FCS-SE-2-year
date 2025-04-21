package events

import (
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/feeding"
	"time"
)

type AnimalMovedEvent struct {
	AnimalID     animal.AnimalID
	OldEnclosure enclosure.EnclosureID
	NewEnclosure enclosure.EnclosureID
	MovedAt      time.Time
}

type FeedingTimeEvent struct {
	AnimalID     animal.AnimalID
	ScheduleID   feeding.ScheduleID
	ScheduledFor time.Time
}
