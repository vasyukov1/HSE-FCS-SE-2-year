package dto

import "hw_02/internal/domain/enclosure"

func MapEnclosureToResponse(e *enclosure.Enclosure) EnclosureResponse {
	return EnclosureResponse{
		ID:            string(e.ID()),
		EnclosureType: string(e.Type()),
		Capacity:      e.Capacity(),
		Animals:       e.GetAnimals(),
	}
}

func MapEnclosuresToResponseList(enclosures []*enclosure.Enclosure) []EnclosureResponse {
	result := make([]EnclosureResponse, 0, len(enclosures))
	for _, e := range enclosures {
		result = append(result, MapEnclosureToResponse(e))
	}
	return result
}
