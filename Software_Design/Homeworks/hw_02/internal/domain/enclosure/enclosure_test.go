package enclosure_test

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
)

func TestNewEnclosure_Valid(t *testing.T) {
	e, err := enclosure.NewEnclosure("e1", "aquarium", 3)
	assert.NoError(t, err)
	assert.Equal(t, enclosure.EnclosureID("e1"), e.ID())
	assert.Equal(t, enclosure.EnclosureType("aquarium"), e.Type())
	assert.Equal(t, 3, e.Capacity())
	assert.Empty(t, e.GetAnimals())
}

func TestNewEnclosure_InvalidCapacity(t *testing.T) {
	e, err := enclosure.NewEnclosure("e2", "herbivore", 0)
	assert.Nil(t, e)
	assert.EqualError(t, err, "invalid enclosure capacity")
}

func TestAddAnimalToEnclosure_Success(t *testing.T) {
	e, _ := enclosure.NewEnclosure("e1", "aquarium", 2)
	err := e.AddAnimalToEnclosure("a1")
	assert.NoError(t, err)
	assert.Equal(t, 1, e.Occupied())
	assert.Equal(t, []animal.AnimalID{"a1"}, e.GetAnimals())
}

func TestAddAnimalToEnclosure_Duplicate(t *testing.T) {
	e, _ := enclosure.NewEnclosure("e1", "aquarium", 2)
	_ = e.AddAnimalToEnclosure("a1")
	err := e.AddAnimalToEnclosure("a1")
	assert.EqualError(t, err, "already have an enclosure")
	assert.Equal(t, 1, e.Occupied())
}

func TestAddAnimalToEnclosure_Full(t *testing.T) {
	e, _ := enclosure.NewEnclosure("e1", "aquarium", 1)
	_ = e.AddAnimalToEnclosure("a1")
	err := e.AddAnimalToEnclosure("a2")
	assert.EqualError(t, err, "enclosure is full")
	assert.Equal(t, 1, e.Occupied())
}

func TestRemoveAnimalFromEnclosure_Success(t *testing.T) {
	e, _ := enclosure.NewEnclosure("e1", "aquarium", 2)
	_ = e.AddAnimalToEnclosure("a1")
	_ = e.AddAnimalToEnclosure("a2")

	err := e.RemoveAnimalFromEnclosure("a1")
	assert.NoError(t, err)
	assert.Equal(t, 1, e.Occupied())
	assert.Equal(t, []animal.AnimalID{"a2"}, e.GetAnimals())
}

func TestRemoveAnimalFromEnclosure_NotFound(t *testing.T) {
	e, _ := enclosure.NewEnclosure("e1", "aquarium", 2)
	_ = e.AddAnimalToEnclosure("a1")

	err := e.RemoveAnimalFromEnclosure("a2")
	assert.EqualError(t, err, "animal not found in enclosure")
	assert.Equal(t, 1, e.Occupied())
}
