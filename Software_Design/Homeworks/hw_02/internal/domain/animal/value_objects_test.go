package animal_test

import (
	"hw_02/internal/domain/animal"
	"testing"
)

func TestNewGender_ValidValues(t *testing.T) {
	male, err := animal.NewGender("male")
	if err != nil {
		t.Errorf("expected no error for 'male', got %v", err)
	}
	if !male.IsMale() {
		t.Errorf("expected gender to be male")
	}

	female, err := animal.NewGender("female")
	if err != nil {
		t.Errorf("expected no error for 'female', got %v", err)
	}
	if female.IsMale() {
		t.Errorf("expected gender to be female")
	}
}

func TestNewGender_InvalidValue(t *testing.T) {
	_, err := animal.NewGender("other")
	if err == nil {
		t.Error("expected error for invalid gender value")
	}
}

func TestNewHealthStatus_ValidValues(t *testing.T) {
	healthy, err := animal.NewHealthStatus("healthy")
	if err != nil {
		t.Errorf("expected no error for 'healthy', got %v", err)
	}
	if !healthy.IsHealthy() {
		t.Errorf("expected IsHealthy to return true")
	}

	sick, err := animal.NewHealthStatus("sick")
	if err != nil {
		t.Errorf("expected no error for 'sick', got %v", err)
	}
	if sick.IsHealthy() {
		t.Errorf("expected IsHealthy to return false for sick status")
	}
}

func TestNewHealthStatus_InvalidValue(t *testing.T) {
	_, err := animal.NewHealthStatus("unknown")
	if err == nil {
		t.Error("expected error for invalid health status value")
	}
}
