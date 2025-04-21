package storage_test

import (
	"context"
	"hw_02/internal/domain/animal"
	"hw_02/internal/infrastructure/storage"
	"testing"
	"time"
)

func TestSaveAnimal(t *testing.T) {
	repo := storage.NewAnimalRepository()
	gender, _ := animal.NewGender("male")
	animalID := animal.AnimalID("1")
	a, _ := animal.NewAnimal(animalID, "Lion", "Simba", time.Now(), gender)

	err := repo.SaveAnimal(context.Background(), a)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedAnimal, err := repo.GetAnimalByID(context.Background(), animalID)
	if err != nil {
		t.Fatalf("expected no error when retrieving saved animal, got %v", err)
	}
	if savedAnimal.ID() != a.ID() {
		t.Errorf("expected animal ID %s, got %s", a.ID(), savedAnimal.ID())
	}
}

func TestGetAllAnimals(t *testing.T) {
	repo := storage.NewAnimalRepository()
	gender, _ := animal.NewGender("female")
	animalID1 := animal.AnimalID("1")
	animalID2 := animal.AnimalID("2")
	a1, _ := animal.NewAnimal(animalID1, "Lion", "Simba", time.Now(), gender)
	a2, _ := animal.NewAnimal(animalID2, "Elephant", "Dumbo", time.Now(), gender)

	err := repo.SaveAnimal(context.Background(), a1)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	err = repo.SaveAnimal(context.Background(), a2)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	animals, err := repo.GetAllAnimals(context.Background())
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	if len(animals) != 2 {
		t.Errorf("expected 2 animals, got %d", len(animals))
	}
}

func TestGetAnimalByID_AnimalExists(t *testing.T) {
	repo := storage.NewAnimalRepository()
	gender, _ := animal.NewGender("male")
	animalID := animal.AnimalID("1")
	a, _ := animal.NewAnimal(animalID, "Lion", "Simba", time.Now(), gender)

	err := repo.SaveAnimal(context.Background(), a)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedAnimal, err := repo.GetAnimalByID(context.Background(), animalID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if savedAnimal.ID() != a.ID() {
		t.Errorf("expected animal ID %s, got %s", a.ID(), savedAnimal.ID())
	}
}

func TestGetAnimalByID_AnimalNotFound(t *testing.T) {
	repo := storage.NewAnimalRepository()

	_, err := repo.GetAnimalByID(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}

func TestDeleteAnimal_AnimalExists(t *testing.T) {
	repo := storage.NewAnimalRepository()
	gender, _ := animal.NewGender("female")
	animalID := animal.AnimalID("1")
	a, _ := animal.NewAnimal(animalID, "Lion", "Simba", time.Now(), gender)

	err := repo.SaveAnimal(context.Background(), a)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	err = repo.DeleteAnimal(context.Background(), animalID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	_, err = repo.GetAnimalByID(context.Background(), animalID)
	if err == nil {
		t.Error("expected error when retrieving deleted animal, got nil")
	}
}

func TestDeleteAnimal_AnimalNotFound(t *testing.T) {
	repo := storage.NewAnimalRepository()

	err := repo.DeleteAnimal(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}
