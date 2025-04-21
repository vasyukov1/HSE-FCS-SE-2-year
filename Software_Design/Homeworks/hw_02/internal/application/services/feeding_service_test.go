package services_test

import (
	"context"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"github.com/stretchr/testify/require"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/events"
	"hw_02/internal/domain/feeding"
	"hw_02/testutils"
	"testing"
	"time"
)

func TestSaveFeeding_Success(t *testing.T) {
	ctx := context.Background()

	feedingID := feeding.ScheduleID("f1")
	animalID := "a1"
	feedingTime := "Monday"
	food := feeding.FoodType("meat")

	f, err := feeding.NewFeeding(feedingID, animalID, feedingTime, food)
	if err != nil {
		require.Error(t, err)
	}

	mockAnimal, err := animal.NewAnimal(animal.AnimalID(animalID), "Tiger", "Tupi", time.Now(), "male")
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	mockAnimalRepo.On("GetAnimalByID", ctx, animal.AnimalID(animalID)).Return(mockAnimal, nil)
	mockAnimalRepo.On("SaveAnimal", ctx, mock.Anything).Return(nil)
	mockFeedingRepo.On("SaveFeeding", ctx, f).Return(nil)

	expectedEvent := events.FeedingTimeEvent{
		AnimalID:     animal.AnimalID(animalID),
		ScheduleID:   feedingID,
		ScheduledFor: time.Now(),
	}

	mockDispatcher.On("Dispatch", mock.MatchedBy(func(evt interface{}) bool {
		e, ok := evt.(events.FeedingTimeEvent)
		return ok && e.AnimalID == expectedEvent.AnimalID && e.ScheduleID == expectedEvent.ScheduleID
	})).Once()

	service := services.NewFeedingService(mockFeedingRepo, mockAnimalRepo, mockDispatcher)
	err = service.SaveFeeding(ctx, f)

	assert.NoError(t, err)
	mockFeedingRepo.AssertExpectations(t)
	mockAnimalRepo.AssertExpectations(t)
	mockDispatcher.AssertExpectations(t)
}

func TestEditFeeding_Success(t *testing.T) {
	ctx := context.Background()

	feedingID := feeding.ScheduleID("f2")
	animalID := "a2"
	feedingTime := "Tuesday Morning"
	food := feeding.FoodType("plants")

	f, err := feeding.NewFeeding(feedingID, animalID, feedingTime, food)
	if err != nil {
		require.Error(t, err)
	}

	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	mockFeedingRepo.On("GetFeedingByID", ctx, feedingID).Return(f, nil)
	mockFeedingRepo.On("SaveFeeding", ctx, f).Return(nil)

	newFeedingTime := "Tuesday Evening"
	newFood := feeding.FoodType("meat")

	expectedEvent := events.FeedingTimeEvent{
		AnimalID:     animal.AnimalID(animalID),
		ScheduleID:   feedingID,
		ScheduledFor: time.Now(),
	}

	mockDispatcher.On("Dispatch", mock.MatchedBy(func(evt interface{}) bool {
		e, ok := evt.(events.FeedingTimeEvent)
		return ok && e.AnimalID == expectedEvent.AnimalID && e.ScheduleID == expectedEvent.ScheduleID
	})).Once()

	service := services.NewFeedingService(mockFeedingRepo, mockAnimalRepo, mockDispatcher)
	err = service.EditFeeding(ctx, feedingID, newFeedingTime, newFood)

	assert.NoError(t, err)
	assert.Equal(t, newFeedingTime, f.Time())
	assert.Equal(t, newFood, f.Food())
	mockAnimalRepo.AssertExpectations(t)
	mockFeedingRepo.AssertExpectations(t)
}

func TestDeleteFeeding_Success(t *testing.T) {
	ctx := context.Background()

	feedingID := feeding.ScheduleID("f3")
	animalID := "a3"
	feedingTime := "Wednesday 11am"
	food := feeding.FoodType("fruit")

	f, err := feeding.NewFeeding(feedingID, animalID, feedingTime, food)
	if err != nil {
		require.Error(t, err)
	}

	mockAnimal, err := animal.NewAnimal(animal.AnimalID(animalID), "Tiger", "Tupi", time.Now(), "male")
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	mockAnimalRepo.On("GetAnimalByID", ctx, animal.AnimalID(animalID)).Return(mockAnimal, nil)
	mockAnimalRepo.On("SaveAnimal", ctx, mock.Anything).Return(nil)
	mockFeedingRepo.On("GetFeedingByID", ctx, feedingID).Return(f, nil)
	mockFeedingRepo.On("DeleteFeeding", ctx, feedingID).Return(nil)

	service := services.NewFeedingService(mockFeedingRepo, mockAnimalRepo, mockDispatcher)
	err = service.DeleteFeeding(ctx, feedingID)

	assert.NoError(t, err)
	mockAnimalRepo.AssertExpectations(t)
	mockFeedingRepo.AssertExpectations(t)
	mockDispatcher.AssertExpectations(t)
}
