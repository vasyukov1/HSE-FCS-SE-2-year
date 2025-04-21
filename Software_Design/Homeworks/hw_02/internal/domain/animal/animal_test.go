package animal_test

import (
	"hw_02/internal/domain/animal"
	"testing"
	"time"
)

func TestNewAnimal_ValidData(t *testing.T) {
	gender, _ := animal.NewGender("male")

	a, err := animal.NewAnimal("1", "Lion", "Simba", time.Now(), gender)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if a.Name() != "Simba" {
		t.Errorf("expected name Simba, got %s", a.Name())
	}
}

func TestNewAnimal_MissingData(t *testing.T) {
	gender, _ := animal.NewGender("female")

	_, err := animal.NewAnimal("2", "", "Milena", time.Now(), gender)
	if err == nil {
		t.Error("expected error due to missing species, got nil")
	}
}

func TestAnimal_HealthyStatus(t *testing.T) {
	gender, _ := animal.NewGender("female")
	a, _ := animal.NewAnimal("4", "Elephant", "Gorge", time.Now(), gender)

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
	a, _ := animal.NewAnimal("5", "Zebra", "Dima", time.Now(), gender)

	newEnclosureId := "e1"
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

	a, _ := animal.NewAnimal(animalID, species, name, birthDate, gender)

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
}

func TestAnimal_IsHealthy(t *testing.T) {
	gender, _ := animal.NewGender("male")
	a, _ := animal.NewAnimal("1", "Tiger", "Barsik", time.Now(), gender)

	if !a.IsHealthy() {
		t.Error("expected animal to be healthy by default")
	}

	a.MakeSick()
	if a.IsHealthy() {
		t.Error("expected animal to be sick after calling MakeSick()")
	}

	a.Heal()
	if !a.IsHealthy() {
		t.Error("expected animal to be healthy after calling Heal()")
	}
}

func TestAnimal_IsMale(t *testing.T) {
	male, _ := animal.NewGender("male")
	female, _ := animal.NewGender("female")

	maleAnimal, _ := animal.NewAnimal("2", "Wolf", "Akela", time.Now(), male)
	femaleAnimal, _ := animal.NewAnimal("3", "Wolf", "Raksha", time.Now(), female)

	if maleAnimal.IsMale() {
		t.Error("expected maleAnimal to be male")
	}

	if femaleAnimal.IsMale() {
		t.Error("expected femaleAnimal to be not male")
	}
}

func TestAnimal_FeedHungry(t *testing.T) {
	gender, _ := animal.NewGender("male")
	a, _ := animal.NewAnimal("7", "Tiger", "Shere Khan", time.Now(), gender)

	if a.Food() {
		t.Error("expected animal to be hungry by default")
	}

	a.Feed()
	if !a.Food() {
		t.Error("expected animal to be fed")
	}

	a.Hungry()
	if a.Food() {
		t.Error("expected animal to be hungry again")
	}
}

func TestAnimal_RemoveFromEnclosure(t *testing.T) {
	gender, _ := animal.NewGender("female")
	a, _ := animal.NewAnimal("8", "Bear", "Masha", time.Now(), gender)

	a.MoveToEnclosure("e2")
	a.RemoveFromEnclosure()

	if a.EnclosureId() != "" {
		t.Errorf("expected empty enclosure ID, got %s", a.EnclosureId())
	}
}

func TestAnimal_SetAndRemoveFeedingSchedule(t *testing.T) {
	gender, _ := animal.NewGender("male")
	a, _ := animal.NewAnimal("9", "Wolf", "Volk", time.Now(), gender)

	a.SetFeedingSchedule("fs1")
	if a.FeedingScheduleID() != "fs1" {
		t.Errorf("expected feeding schedule fs1, got %s", a.FeedingScheduleID())
	}

	a.RemoveFeedingSchedule()
	if a.FeedingScheduleID() != "" {
		t.Errorf("expected empty feeding schedule ID, got %s", a.FeedingScheduleID())
	}
}
