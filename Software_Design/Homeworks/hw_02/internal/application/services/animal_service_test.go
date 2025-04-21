package services_test

import (
	"context"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"hw_02/testutils"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMakeSick(t *testing.T) {
	ctx := context.Background()
	mockAnimal := testutils.MustAnimal("a1", "male")

	repo := new(testutils.MockAnimalRepo)
	repo.On("GetAnimalByID", ctx, mockAnimal.ID()).Return(mockAnimal, nil)

	service := services.NewAnimalService(repo, nil)
	err := service.MakeSick(ctx, mockAnimal.ID())

	assert.NoError(t, err)
	assert.Equal(t, animal.Sick, mockAnimal.HealthStatus())
}

func TestHeal(t *testing.T) {
	ctx := context.Background()
	mockAnimal := testutils.MustAnimal("a2", "female")
	mockAnimal.MakeSick()

	repo := new(testutils.MockAnimalRepo)
	repo.On("GetAnimalByID", ctx, mockAnimal.ID()).Return(mockAnimal, nil)

	service := services.NewAnimalService(repo, nil)
	err := service.Heal(ctx, mockAnimal.ID())

	assert.NoError(t, err)
	assert.Equal(t, animal.Healthy, mockAnimal.HealthStatus())
}

func TestFeed_Success(t *testing.T) {
	ctx := context.Background()
	mockAnimal := testutils.MustAnimal("a3", "male")
	mockAnimal.SetFeedingSchedule("f3")
	mockFeeding := testutils.MustFeeding("f3", string(mockAnimal.ID()), "08:00", "meat")

	animalRepo := new(testutils.MockAnimalRepo)
	feedingRepo := new(testutils.MockFeedingRepo)

	animalRepo.On("GetAnimalByID", ctx, mockAnimal.ID()).Return(mockAnimal, nil)
	feedingRepo.On("GetFeedingByID", ctx, feeding.ScheduleID("f3")).Return(mockFeeding, nil)

	service := services.NewAnimalService(animalRepo, feedingRepo)
	err := service.Feed(ctx, mockAnimal.ID(), "meat", "08:00")

	assert.NoError(t, err)
	assert.True(t, mockAnimal.Food())
}

func TestFeed_WrongTime(t *testing.T) {
	ctx := context.Background()
	mockAnimal := testutils.MustAnimal("a4", "female")
	mockAnimal.SetFeedingSchedule("f4")
	mockFeeding := testutils.MustFeeding("f4", string(mockAnimal.ID()), "08:00", feeding.FoodType("meat"))

	animalRepo := new(testutils.MockAnimalRepo)
	feedingRepo := new(testutils.MockFeedingRepo)

	animalRepo.On("GetAnimalByID", ctx, mockAnimal.ID()).Return(mockAnimal, nil)
	feedingRepo.On("GetFeedingByID", ctx, feeding.ScheduleID("f4")).Return(mockFeeding, nil)

	service := services.NewAnimalService(animalRepo, feedingRepo)
	err := service.Feed(ctx, mockAnimal.ID(), "meat", "09:00")

	assert.ErrorContains(t, err, "time does not match")
}

func TestHungry(t *testing.T) {
	ctx := context.Background()
	mockAnimal := testutils.MustAnimal("a5", "male")
	mockAnimal.Feed()

	repo := new(testutils.MockAnimalRepo)
	repo.On("GetAnimalByID", ctx, mockAnimal.ID()).Return(mockAnimal, nil)

	service := services.NewAnimalService(repo, nil)
	err := service.Hungry(ctx, mockAnimal.ID())

	assert.NoError(t, err)
	assert.False(t, mockAnimal.Food())
}
