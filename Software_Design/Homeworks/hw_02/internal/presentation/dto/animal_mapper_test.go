package dto_test

import (
	"testing"
	"time"

	"github.com/stretchr/testify/require"

	"hw_02/internal/domain/animal"
	"hw_02/internal/presentation/dto"
)

func TestMapAnimalToResponse(t *testing.T) {
	id := animal.AnimalID("a123")
	species := "Tiger"
	name := "Sher Khan"
	birthDate := time.Date(2015, time.July, 10, 0, 0, 0, 0, time.UTC)
	gender := animal.Male

	a, _ := animal.NewAnimal(id, species, name, birthDate, gender)

	resp := dto.MapAnimalToResponse(a)

	require.Equal(t, "a123", resp.ID)
	require.Equal(t, "Tiger", resp.Species)
	require.Equal(t, "Sher Khan", resp.Name)
	require.Equal(t, birthDate, resp.BirthDate)
	require.Equal(t, "male", resp.Gender)
	require.Equal(t, "healthy", resp.HealthStatus)
}

func TestMapAnimalsToResponseList(t *testing.T) {
	a1, _ := animal.NewAnimal("1", "Lion", "Simba", time.Now(), animal.Male)
	a2, _ := animal.NewAnimal("2", "Elephant", "Dumbo", time.Now(), animal.Female)

	list := []*animal.Animal{a1, a2}
	respList := dto.MapAnimalsToResponseList(list)

	require.Len(t, respList, 2)
	require.Equal(t, "1", respList[0].ID)
	require.Equal(t, "2", respList[1].ID)
	require.Equal(t, "Lion", respList[0].Species)
	require.Equal(t, "Elephant", respList[1].Species)
}
