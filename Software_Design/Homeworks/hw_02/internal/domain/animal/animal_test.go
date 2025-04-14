package animal_test

import (
	"hw_02/internal/domain/animal"
	"testing"
	"time"
)

func TestNewAnimal_ValidData(t *testing.T) {
	gender, _ := animal.NewGender("male")
	foodType, _ := animal.NewFoodType("meat")

	a, err := animal.NewAnimal("1", "Lion", "Simba", time.Now(), gender, foodType)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if a.Name() != "Simba" {
		t.Errorf("expected name Simba, got %s", a.Name())
	}
}

func TestNewAnimal_MissingData(t *testing.T) {
	gender, _ := animal.NewGender("female")
	foodType, _ := animal.NewFoodType("plants")

	_, err := animal.NewAnimal("2", "", "Milena", time.Now(), gender, foodType)
	if err == nil {
		t.Error("expected error due to missing species, got nil")
	}
}
