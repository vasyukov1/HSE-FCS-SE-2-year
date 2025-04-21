package feeding

import "errors"

//
// FoodType VO
//

type FoodType string

const (
	Meat   FoodType = "meat"
	Plants FoodType = "plants"
	Fish   FoodType = "fish"
	Fruit  FoodType = "fruit"
)

func NewFoodType(value string) (FoodType, error) {
	switch value {
	case string(Meat), string(Plants), string(Fish), string(Fruit):
		return FoodType(value), nil
	default:
		return "", errors.New("invalid food type")
	}
}
