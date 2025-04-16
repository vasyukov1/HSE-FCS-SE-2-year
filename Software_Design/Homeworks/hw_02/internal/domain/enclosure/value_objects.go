package enclosure

import "errors"

//
// EnclosureType VO
//

type EnclosureType string

const (
	Predator  EnclosureType = "predator"
	Herbivore EnclosureType = "herbivore"
	Bird      EnclosureType = "bird"
	Aquarium  EnclosureType = "aquarium"
)

func NewEnclosureType(value string) (EnclosureType, error) {
	switch value {
	case string(Predator), string(Herbivore), string(Bird), string(Aquarium):
		return EnclosureType(value), nil
	default:
		return "", errors.New("invalid enclosure type")
	}
}
