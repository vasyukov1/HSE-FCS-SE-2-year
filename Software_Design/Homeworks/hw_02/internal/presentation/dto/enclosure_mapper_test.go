package dto_test

import (
	"hw_02/internal/domain/animal"
	"testing"

	"github.com/stretchr/testify/require"

	"hw_02/internal/domain/enclosure"
	"hw_02/internal/presentation/dto"
)

func TestMapEnclosureToResponse(t *testing.T) {
	id := enclosure.EnclosureID("e1")
	typ := enclosure.Bird
	capacity := 5
	animalIDs := []animal.AnimalID{"a1", "a2"}

	e, _ := enclosure.NewEnclosure(id, typ, capacity)
	for _, animalID := range animalIDs {
		err := e.AddAnimalToEnclosure(animalID)
		require.NoError(t, err)
	}

	resp := dto.MapEnclosureToResponse(e)

	require.Equal(t, "e1", resp.ID)
	require.Equal(t, "bird", resp.EnclosureType)
	require.Equal(t, 5, resp.Capacity)
	require.ElementsMatch(t, animalIDs, resp.Animals)
}

func TestMapEnclosuresToResponseList(t *testing.T) {
	e1, _ := enclosure.NewEnclosure("e1", enclosure.Predator, 3)
	e2, _ := enclosure.NewEnclosure("e2", enclosure.Aquarium, 10)

	_ = e1.AddAnimalToEnclosure("a1")
	_ = e2.AddAnimalToEnclosure("a2")

	list := []*enclosure.Enclosure{e1, e2}
	respList := dto.MapEnclosuresToResponseList(list)

	require.Len(t, respList, 2)
	require.Equal(t, "e1", respList[0].ID)
	require.Equal(t, "predator", respList[0].EnclosureType)
	require.Equal(t, 3, respList[0].Capacity)
	require.ElementsMatch(t, []animal.AnimalID{"a1"}, respList[0].Animals)

	require.Equal(t, "e2", respList[1].ID)
	require.Equal(t, "aquarium", respList[1].EnclosureType)
	require.Equal(t, 10, respList[1].Capacity)
	require.ElementsMatch(t, []animal.AnimalID{"a2"}, respList[1].Animals)
}
