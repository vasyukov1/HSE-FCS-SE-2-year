package storage_test

import (
	"context"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/infrastructure/storage"
	"testing"
)

func TestSaveEnclosure(t *testing.T) {
	repo := storage.NewEnclosureRepository()
	enclosureID := enclosure.EnclosureID("1")
	enclosureType, _ := enclosure.NewEnclosureType("predator")
	e, _ := enclosure.NewEnclosure(enclosureID, enclosureType, 10)

	err := repo.SaveEnclosure(context.Background(), e)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedEnclosure, err := repo.GetEnclosureByID(context.Background(), enclosureID)
	if err != nil {
		t.Fatalf("expected no error when retrieving saved enclosure, got %v", err)
	}
	if savedEnclosure.ID() != e.ID() {
		t.Errorf("expected enclosure ID %s, got %s", e.ID(), savedEnclosure.ID())
	}
}

func TestGetAllEnclosures(t *testing.T) {
	repo := storage.NewEnclosureRepository()
	enclosureID1 := enclosure.EnclosureID("1")
	enclosureID2 := enclosure.EnclosureID("2")
	enclosureType, _ := enclosure.NewEnclosureType("predator")
	e1, _ := enclosure.NewEnclosure(enclosureID1, enclosureType, 10)
	e2, _ := enclosure.NewEnclosure(enclosureID2, enclosureType, 5)

	err := repo.SaveEnclosure(context.Background(), e1)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	err = repo.SaveEnclosure(context.Background(), e2)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	enclosures, err := repo.GetAllEnclosure(context.Background())
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	if len(enclosures) != 2 {
		t.Errorf("expected 2 enclosures, got %d", len(enclosures))
	}
}

func TestGetEnclosureByID_EnclosureExists(t *testing.T) {
	repo := storage.NewEnclosureRepository()
	enclosureID := enclosure.EnclosureID("1")
	enclosureType, _ := enclosure.NewEnclosureType("herbivore")
	e, _ := enclosure.NewEnclosure(enclosureID, enclosureType, 7)

	err := repo.SaveEnclosure(context.Background(), e)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedEnclosure, err := repo.GetEnclosureByID(context.Background(), enclosureID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if savedEnclosure.ID() != e.ID() {
		t.Errorf("expected enclosure ID %s, got %s", e.ID(), savedEnclosure.ID())
	}
}

func TestGetEnclosureByID_EnclosureNotFound(t *testing.T) {
	repo := storage.NewEnclosureRepository()

	_, err := repo.GetEnclosureByID(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}

func TestDeleteEnclosure_EnclosureExists(t *testing.T) {
	repo := storage.NewEnclosureRepository()
	enclosureID := enclosure.EnclosureID("1")
	enclosureType, _ := enclosure.NewEnclosureType("aquarium")
	e, _ := enclosure.NewEnclosure(enclosureID, enclosureType, 10)

	err := repo.SaveEnclosure(context.Background(), e)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	err = repo.DeleteEnclosure(context.Background(), enclosureID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	_, err = repo.GetEnclosureByID(context.Background(), enclosureID)
	if err == nil {
		t.Error("expected error when retrieving deleted enclosure, got nil")
	}
}

func TestDeleteEnclosure_EnclosureNotFound(t *testing.T) {
	repo := storage.NewEnclosureRepository()

	err := repo.DeleteEnclosure(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}
