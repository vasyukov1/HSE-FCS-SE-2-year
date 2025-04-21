package feeding_test

import (
	"hw_02/internal/domain/feeding"
	"testing"
)

func TestNewFoodType_ValidValues(t *testing.T) {
	tests := []struct {
		input    string
		expected feeding.FoodType
	}{
		{"meat", feeding.Meat},
		{"plants", feeding.Plants},
		{"fish", feeding.Fish},
		{"fruit", feeding.Fruit},
	}

	for _, tt := range tests {
		result, err := feeding.NewFoodType(tt.input)
		if err != nil {
			t.Errorf("expected no error for input %s, got %v", tt.input, err)
		}
		if result != tt.expected {
			t.Errorf("expected %s, got %s", tt.expected, result)
		}
	}
}

func TestNewFoodType_InvalidValue(t *testing.T) {
	_, err := feeding.NewFoodType("bread")
	if err == nil {
		t.Error("expected error for invalid food type 'bread', got nil")
	}
}
