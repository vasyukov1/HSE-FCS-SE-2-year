package animal_test

import (
	"hw_02/internal/domain/animal"
	"testing"
	"time"
)

func TestNewAnimal_ValidData(t *testing.T) {
	gender, _ := animal.NewGender("male")
	food, _ := animal.NewFoodType("meat")

	a, err := animal.NewAnimal("1", "Lion", "Simba", time.Now(), gender, food)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if a.Name() != "Simba" {
		t.Errorf("expected name Simba, got %s", a.Name())
	}
}

func TestNewAnimal_MissingData(t *testing.T) {
	gender, _ := animal.NewGender("female")
	food, _ := animal.NewFoodType("plants")

	_, err := animal.NewAnimal("2", "", "Milena", time.Now(), gender, food)
	if err == nil {
		t.Error("expected error due to missing species, got nil")
	}
}

func TestAnimal_Feed(t *testing.T) {
	gender, _ := animal.NewGender("male")
	food, _ := animal.NewFoodType("meat")
	a, _ := animal.NewAnimal("3", "Tiger", "Fred", time.Now(), gender, food)

	if !a.Feed(food) {
		t.Error("expected feeding to succeed with correct food type")
	}

	wrongFood, _ := animal.NewFoodType("fruit")
	if a.Feed(wrongFood) {
		t.Error("expected feeding to fail with incorrect food type")
	}
}

func TestAnimal_HealthyStatus(t *testing.T) {
	gender, _ := animal.NewGender("female")
	food, _ := animal.NewFoodType("fish")
	a, _ := animal.NewAnimal("4", "Elephant", "Gorge", time.Now(), gender, food)

	if !a.HealthStatus().IsHealthy() {
		t.Error("expected animal to be healthy by default")
	}

	a.MakeSick()
	if a.HealthStatus().IsHealthy() {
		t.Error("expected animal to be sick after MakeSick")
	}

	a.Heal()
	if !a.HealthStatus().IsHealthy() {
		t.Error("expected animal to be healthy after Heal")
	}
}

func TestAnimal_MoveToEnclosure(t *testing.T) {
	gender, _ := animal.NewGender("male")
	food, _ := animal.NewFoodType("plants")
	a, _ := animal.NewAnimal("5", "Zebra", "Dima", time.Now(), gender, food)

	newEnclosureId := animal.EnclosureID("e1")
	a.MoveToEnclosure(newEnclosureId)
	if a.EnclosureId() != newEnclosureId {
		t.Errorf("expected enclosure ID %s, got %s", newEnclosureId, a.EnclosureId())
	}
}

func TestAnimal_Data(t *testing.T) {
	animalID := animal.AnimalID("6")
	species := "Zebra"
	name := "Leonid"
	birthDate := time.Now()
	gender, _ := animal.NewGender("male")
	food, _ := animal.NewFoodType("plants")

	a, _ := animal.NewAnimal(animalID, species, name, birthDate, gender, food)

	if a.ID() != animalID {
		t.Errorf("expected animal ID %s, got %s", animalID, a.ID())
	}

	if a.Species() != species {
		t.Errorf("expected species %s, got %s", species, a.Species())
	}

	if a.Name() != name {
		t.Errorf("expected name %s, got %s", name, a.Name())
	}

	if a.BirthDate() != birthDate {
		t.Errorf("expected birthDate %s, got %s", birthDate, a.BirthDate())
	}

	if a.Gender() != gender {
		t.Errorf("expected gender %s, got %s", gender, a.Gender())
	}

	if !a.Gender().IsMale() {
		t.Errorf("expected gender to be male, got female")
	}

	if a.Food() != food {
		t.Errorf("expected food %s, got %s", food, a.Food())
	}
}
