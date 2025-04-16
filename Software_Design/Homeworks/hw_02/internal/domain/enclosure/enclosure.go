package enclosure

import (
	"errors"
	"hw_02/internal/domain/animal"
)

type Enclosure struct {
	id            EnclosureID
	enclosureType EnclosureType
	capacity      int
	animals       []animal.AnimalID
}

func NewEnclosure(id EnclosureID, enclosureType EnclosureType, capacity int) (*Enclosure, error) {
	if capacity <= 0 {
		return nil, errors.New("invalid enclosure capacity")
	}
	return &Enclosure{
		id:            id,
		enclosureType: enclosureType,
		capacity:      capacity,
		animals:       []animal.AnimalID{},
	}, nil
}

func (e *Enclosure) AddAnimalToEnclosure(animalID animal.AnimalID) error {
	if len(e.animals) >= e.capacity {
		return errors.New("enclosure is full")
	}

	for _, animalIDinEnclosure := range e.animals {
		if animalIDinEnclosure == animalID {
			return errors.New("already have an enclosure")
		}
	}

	e.animals = append(e.animals, animalID)
	return nil
}

func (e *Enclosure) RemoveAnimalFromEnclosure(animalID animal.AnimalID) error {
	for i, id := range e.animals {
		if id == animalID {
			e.animals = append(e.animals[:i], e.animals[i+1:]...)
			return nil
		}
	}
	return errors.New("animal not found in enclosure")
}

func (e *Enclosure) Occupied() int {
	return len(e.animals)
}

func (e *Enclosure) ID() EnclosureID               { return e.id }
func (e *Enclosure) Type() EnclosureType           { return e.enclosureType }
func (e *Enclosure) Capacity() int                 { return e.capacity }
func (e *Enclosure) GetAnimals() []animal.AnimalID { return e.animals }
