package enclosure_test

import (
	"hw_02/internal/domain/enclosure"
	"testing"
)

func TestNewEnclosureType_ValidValues(t *testing.T) {
	tests := []struct {
		input    string
		expected enclosure.EnclosureType
	}{
		{"predator", enclosure.Predator},
		{"herbivore", enclosure.Herbivore},
		{"bird", enclosure.Bird},
		{"aquarium", enclosure.Aquarium},
	}

	for _, tt := range tests {
		result, err := enclosure.NewEnclosureType(tt.input)
		if err != nil {
			t.Errorf("expected no error for input %s, got %v", tt.input, err)
		}
		if result != tt.expected {
			t.Errorf("expected %s, got %s", tt.expected, result)
		}
	}
}

func TestNewEnclosureType_InvalidValue(t *testing.T) {
	_, err := enclosure.NewEnclosureType("reptile")
	if err == nil {
		t.Error("expected error for invalid enclosure type 'reptile', got nil")
	}
}
