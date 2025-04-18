package dto

import "hw_02/internal/domain/feeding"

func MapFeedingToResponse(f *feeding.Feeding) FeedingResponse {
	return FeedingResponse{
		ID:       string(f.ID()),
		AnimalID: f.AnimalID(),
		Time:     f.Time(),
		Food:     string(f.Food()),
	}
}

func MapFeedingsToResponseList(feedings []*feeding.Feeding) []FeedingResponse {
	result := make([]FeedingResponse, 0, len(feedings))
	for _, f := range feedings {
		result = append(result, MapFeedingToResponse(f))
	}
	return result
}
