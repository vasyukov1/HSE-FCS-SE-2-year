package services_test

import (
	"context"
	"errors"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/feeding"
	"hw_02/testutils"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
)

func TestStatsService_GetAnimals(t *testing.T) {
	ctx := context.Background()
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockEnclosureRepo := new(testutils.MockEnclosureRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)

	expectedAnimals := []*animal.Animal{
		MustNewAnimal("a1", "Lion", "Simba", time.Now(), "male"),
	}

	mockAnimalRepo.On("GetAllAnimals", ctx).Return(expectedAnimals, nil)

	service := services.NewStatsService(mockAnimalRepo, mockEnclosureRepo, mockFeedingRepo)
	animals := service.GetAnimals(ctx)

	assert.Equal(t, expectedAnimals, animals)
	mockAnimalRepo.AssertExpectations(t)
}

func TestStatsService_GetAnimals_Error(t *testing.T) {
	ctx := context.Background()
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockEnclosureRepo := new(testutils.MockEnclosureRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)

	mockAnimalRepo.On("GetAllAnimals", ctx).Return([]*animal.Animal{}, errors.New("db error"))

	service := services.NewStatsService(mockAnimalRepo, mockEnclosureRepo, mockFeedingRepo)
	animals := service.GetAnimals(ctx)

	assert.Empty(t, animals)
	mockAnimalRepo.AssertExpectations(t)
}

func TestStatsService_GetEnclosures(t *testing.T) {
	ctx := context.Background()
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockEnclosureRepo := new(testutils.MockEnclosureRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)

	mockEnclosureRepo.On("GetAllEnclosure", ctx).Return([]*enclosure.Enclosure{}, nil)

	service := services.NewStatsService(mockAnimalRepo, mockEnclosureRepo, mockFeedingRepo)
	enclosures := service.GetEnclosures(ctx)

	assert.NotNil(t, enclosures)
	mockEnclosureRepo.AssertExpectations(t)
}

func TestStatsService_GetFeedings(t *testing.T) {
	ctx := context.Background()
	mockAnimalRepo := new(testutils.MockAnimalRepo)
	mockEnclosureRepo := new(testutils.MockEnclosureRepo)
	mockFeedingRepo := new(testutils.MockFeedingRepo)

	mockFeedingRepo.On("GetAllFeedings", ctx).Return([]*feeding.Feeding{}, nil)

	service := services.NewStatsService(mockAnimalRepo, mockEnclosureRepo, mockFeedingRepo)
	feedings := service.GetFeedings(ctx)

	assert.NotNil(t, feedings)
	mockFeedingRepo.AssertExpectations(t)
}

func MustNewAnimal(id, species, name string, birth time.Time, gender string) *animal.Animal {
	g, _ := animal.NewGender(gender)
	a, _ := animal.NewAnimal(animal.AnimalID(id), species, name, birth, g)
	return a
}
