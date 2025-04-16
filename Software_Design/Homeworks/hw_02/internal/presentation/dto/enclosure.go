package dto

import "hw_02/internal/domain/animal"

type EnclosureResponse struct {
	ID            string            `json:"id"`
	EnclosureType string            `json:"enclosure_type"`
	Capacity      int               `json:"capacity"`
	Animals       []animal.AnimalID `json:"animals"`
}
