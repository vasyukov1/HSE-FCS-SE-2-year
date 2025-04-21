package dto_test

import (
	"github.com/stretchr/testify/require"
	"testing"

	"hw_02/internal/domain/feeding"
	"hw_02/internal/presentation/dto"
)

func TestMapFeedingToResponse(t *testing.T) {
	id := feeding.ScheduleID("f1")
	animalID := "a1"
	timeStr := "Now"
	food := feeding.Meat

	f, _ := feeding.NewFeeding(id, animalID, timeStr, food)

	resp := dto.MapFeedingToResponse(f)

	require.Equal(t, "f1", resp.ID)
	require.Equal(t, animalID, resp.AnimalID)
	require.Equal(t, timeStr, resp.Time)
	require.Equal(t, "meat", resp.Food)
}

func TestMapFeedingsToResponseList(t *testing.T) {
	f1, _ := feeding.NewFeeding("f1", "a1", "Now", feeding.Meat)
	f2, _ := feeding.NewFeeding("f2", "a2", "Yesterday", feeding.Fruit)

	list := []*feeding.Feeding{f1, f2}
	respList := dto.MapFeedingsToResponseList(list)

	require.Len(t, respList, 2)

	require.Equal(t, "f1", respList[0].ID)
	require.Equal(t, "a1", respList[0].AnimalID)
	require.Equal(t, "Now", respList[0].Time)
	require.Equal(t, "meat", respList[0].Food)

	require.Equal(t, "f2", respList[1].ID)
	require.Equal(t, "a2", respList[1].AnimalID)
	require.Equal(t, "Yesterday", respList[1].Time)
	require.Equal(t, "fruit", respList[1].Food)
}
