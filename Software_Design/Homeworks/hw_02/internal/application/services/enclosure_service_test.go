package services_test

import (
	"context"
	"errors"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/events"
	"hw_02/testutils"
	"testing"
)

func TestSaveEnclosure(t *testing.T) {
	ctx := context.Background()

	mockRepo := new(testutils.MockEnclosureRepo)
	mockAnimal := new(testutils.MockAnimalRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	enc := testutils.MustEnclosure("e1", "predator", 5)

	mockRepo.On("SaveEnclosure", ctx, enc).Return(nil)

	service := services.NewEnclosureService(mockRepo, mockAnimal, mockDispatcher)
	err := service.SaveEnclosure(ctx, enc)

	assert.NoError(t, err)
	mockRepo.AssertExpectations(t)
}

func TestGetAllEnclosure(t *testing.T) {
	ctx := context.Background()

	mockRepo := new(testutils.MockEnclosureRepo)
	mockAnimal := new(testutils.MockAnimalRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	enclosures := []*enclosure.Enclosure{
		testutils.MustEnclosure("e1", "bird", 10),
		testutils.MustEnclosure("e2", "aquarium", 20),
	}
	mockRepo.On("GetAllEnclosure", ctx).Return(enclosures, nil)

	service := services.NewEnclosureService(mockRepo, mockAnimal, mockDispatcher)
	res, err := service.GetAllEnclosure(ctx)

	assert.NoError(t, err)
	assert.Equal(t, enclosures, res)
	mockRepo.AssertExpectations(t)
}

func TestAddAnimalToEnclosure_Success(t *testing.T) {
	ctx := context.Background()

	enclosureID := enclosure.EnclosureID("e1")
	oldEnclosureID := enclosure.EnclosureID("old")
	animalID := animal.AnimalID("a1")

	mockAnimal := new(testutils.MockAnimalRepo)
	mockEnclosure := new(testutils.MockEnclosureRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	a := testutils.MustAnimal(string(animalID), "male")

	newE := testutils.MustEnclosure(enclosureID, "aquarium", 4)
	oldE := testutils.MustEnclosure(oldEnclosureID, "herbivore", 1)
	_ = oldE.AddAnimalToEnclosure(a.ID())
	a.MoveToEnclosure(string(oldEnclosureID))

	mockAnimal.On("GetAnimalByID", ctx, animalID).Return(a, nil)
	mockEnclosure.On("GetEnclosureByID", ctx, enclosureID).Return(newE, nil)
	mockEnclosure.On("GetEnclosureByID", ctx, oldEnclosureID).Return(oldE, nil)
	mockAnimal.On("SaveAnimal", ctx, a).Return(nil)
	mockEnclosure.On("SaveEnclosure", ctx, newE).Return(nil)

	mockDispatcher.On("Dispatch", mock.MatchedBy(func(evt interface{}) bool {
		e, ok := evt.(events.AnimalMovedEvent)
		return ok &&
			e.AnimalID == animalID &&
			e.OldEnclosure == oldEnclosureID &&
			e.NewEnclosure == enclosureID
	})).Return()

	service := services.NewEnclosureService(mockEnclosure, mockAnimal, mockDispatcher)
	err := service.AddAnimalToEnclosure(ctx, enclosureID, animalID)

	assert.NoError(t, err)
	mockAnimal.AssertExpectations(t)
	mockEnclosure.AssertExpectations(t)
	mockDispatcher.AssertExpectations(t)
}

func TestRemoveAnimalFromEnclosure_Success(t *testing.T) {
	ctx := context.Background()

	enclosureID := enclosure.EnclosureID("e1")
	animalID := animal.AnimalID("a1")

	mockAnimal := new(testutils.MockAnimalRepo)
	mockEnclosure := new(testutils.MockEnclosureRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	a := testutils.MustAnimal(string(animalID), "male")
	e := testutils.MustEnclosure(enclosureID, "herbivore", 7)
	_ = e.AddAnimalToEnclosure(a.ID())

	mockAnimal.On("GetAnimalByID", ctx, animalID).Return(a, nil)
	mockEnclosure.On("GetEnclosureByID", ctx, enclosureID).Return(e, nil)
	mockAnimal.On("SaveAnimal", ctx, a).Return(nil)
	mockEnclosure.On("SaveEnclosure", ctx, e).Return(nil)

	service := services.NewEnclosureService(mockEnclosure, mockAnimal, mockDispatcher)
	err := service.RemoveAnimalFromEnclosure(ctx, enclosureID, animalID)

	assert.NoError(t, err)
	mockAnimal.AssertExpectations(t)
	mockEnclosure.AssertExpectations(t)
}

func TestAddAnimalToEnclosure_AnimalNotFound(t *testing.T) {
	ctx := context.Background()

	mockAnimal := new(testutils.MockAnimalRepo)
	mockEnclosure := new(testutils.MockEnclosureRepo)
	mockDispatcher := new(testutils.MockDispatcher)

	mockAnimal.On("GetAnimalByID", ctx, animal.AnimalID("a1")).Return((*animal.Animal)(nil), errors.New("not found"))

	service := services.NewEnclosureService(mockEnclosure, mockAnimal, mockDispatcher)
	err := service.AddAnimalToEnclosure(ctx, "e1", "a1")

	assert.EqualError(t, err, "animal not found")
}
