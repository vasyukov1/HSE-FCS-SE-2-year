package dto_test

import (
	"testing"

	"github.com/stretchr/testify/require"

	"hw_02/internal/presentation/dto"
)

func TestMapGeneralStatsToResponse(t *testing.T) {
	animals := 12
	enclosures := 5
	feedings := 20

	resp := dto.MapGeneralStatsToResponse(animals, enclosures, feedings)

	require.Equal(t, animals, resp.Animals)
	require.Equal(t, enclosures, resp.Enclosures)
	require.Equal(t, feedings, resp.Feedings)
}

func TestMapStatsToResponse(t *testing.T) {
	resp := dto.MapStatsToResponse(
		10, 7, 3, 6, 4, 8, 2,
		[]string{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "a10"},
		[]string{"a1", "a2", "a3", "a4", "a5", "a6", "a7"},
		[]string{"a8", "a9", "a10"},
		[]string{"a1", "a3", "a5", "a7", "a9", "a10"},
		[]string{"a2", "a4", "a6", "a8"},
		[]string{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8"},
		[]string{"a9", "a10"},
	)

	require.Equal(t, 10, resp.Total)
	require.Equal(t, 7, resp.Healthy)
	require.Equal(t, 3, resp.Seek)
	require.Equal(t, 6, resp.Male)
	require.Equal(t, 4, resp.Female)
	require.Equal(t, 8, resp.WithEnclosure)
	require.Equal(t, 2, resp.WithoutEnclosure)

	require.ElementsMatch(t, []string{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "a10"}, resp.AnimalIDs)
	require.ElementsMatch(t, []string{"a1", "a2", "a3", "a4", "a5", "a6", "a7"}, resp.HealthyIDs)
	require.ElementsMatch(t, []string{"a8", "a9", "a10"}, resp.SickIDs)
	require.ElementsMatch(t, []string{"a1", "a3", "a5", "a7", "a9", "a10"}, resp.MaleIDs)
	require.ElementsMatch(t, []string{"a2", "a4", "a6", "a8"}, resp.FemaleIDs)
	require.ElementsMatch(t, []string{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8"}, resp.WithEnclosureIDs)
	require.ElementsMatch(t, []string{"a9", "a10"}, resp.WithoutEnclosureIDs)
}
