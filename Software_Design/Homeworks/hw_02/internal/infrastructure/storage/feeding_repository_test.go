package storage_test

import (
	"context"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"hw_02/internal/infrastructure/storage"
	"testing"
)

func TestSaveFeeding(t *testing.T) {
	repo := storage.NewFeedingRepository()
	feedingID := feeding.ScheduleID("1")
	animalID := "1"
	foodType, _ := feeding.NewFoodType("meat")
	f, _ := feeding.NewFeeding(feedingID, animalID, "2025-04-21 08:00", foodType)

	err := repo.SaveFeeding(context.Background(), f)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedFeeding, err := repo.GetFeedingByID(context.Background(), feedingID)
	if err != nil {
		t.Fatalf("expected no error when retrieving saved feeding, got %v", err)
	}
	if savedFeeding.ID() != f.ID() {
		t.Errorf("expected feeding ID %s, got %s", f.ID(), savedFeeding.ID())
	}
}

func TestGetAllFeedings(t *testing.T) {
	repo := storage.NewFeedingRepository()
	feedingID1 := feeding.ScheduleID("1")
	feedingID2 := feeding.ScheduleID("2")
	animalID := "1"
	foodType, _ := feeding.NewFoodType("meat")
	f1, _ := feeding.NewFeeding(feedingID1, animalID, "2025-04-21 08:00", foodType)
	f2, _ := feeding.NewFeeding(feedingID2, animalID, "2025-04-21 12:00", foodType)

	err := repo.SaveFeeding(context.Background(), f1)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	err = repo.SaveFeeding(context.Background(), f2)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	feedings, err := repo.GetAllFeedings(context.Background())
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	if len(feedings) != 2 {
		t.Errorf("expected 2 feedings, got %d", len(feedings))
	}
}

func TestGetFeedingByID_FeedingExists(t *testing.T) {
	repo := storage.NewFeedingRepository()
	feedingID := feeding.ScheduleID("1")
	animalID := "1"
	foodType, _ := feeding.NewFoodType("fish")
	f, _ := feeding.NewFeeding(feedingID, animalID, "2025-04-21 10:00", foodType)

	err := repo.SaveFeeding(context.Background(), f)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedFeeding, err := repo.GetFeedingByID(context.Background(), feedingID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if savedFeeding.ID() != f.ID() {
		t.Errorf("expected feeding ID %s, got %s", f.ID(), savedFeeding.ID())
	}
}

func TestGetFeedingByID_FeedingNotFound(t *testing.T) {
	repo := storage.NewFeedingRepository()

	_, err := repo.GetFeedingByID(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}

func TestGetFeedingByAnimalID(t *testing.T) {
	repo := storage.NewFeedingRepository()
	feedingID := feeding.ScheduleID("1")
	animalID := "1"
	foodType, _ := feeding.NewFoodType("fruit")
	f, _ := feeding.NewFeeding(feedingID, animalID, "2025-04-21 09:00", foodType)

	err := repo.SaveFeeding(context.Background(), f)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	savedFeeding, err := repo.GetFeedingByAnimalID(context.Background(), animal.AnimalID(animalID))
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if savedFeeding.ID() != f.ID() {
		t.Errorf("expected feeding ID %s, got %s", f.ID(), savedFeeding.ID())
	}
}

func TestGetFeedingByAnimalID_FeedingNotFound(t *testing.T) {
	repo := storage.NewFeedingRepository()

	_, err := repo.GetFeedingByAnimalID(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}

func TestDeleteFeeding_FeedingExists(t *testing.T) {
	repo := storage.NewFeedingRepository()
	feedingID := feeding.ScheduleID("1")
	animalID := "1"
	foodType, _ := feeding.NewFoodType("meat")
	f, _ := feeding.NewFeeding(feedingID, animalID, "2025-04-21 08:00", foodType)

	err := repo.SaveFeeding(context.Background(), f)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	err = repo.DeleteFeeding(context.Background(), feedingID)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	_, err = repo.GetFeedingByID(context.Background(), feedingID)
	if err == nil {
		t.Error("expected error when retrieving deleted feeding, got nil")
	}
}

func TestDeleteFeeding_FeedingNotFound(t *testing.T) {
	repo := storage.NewFeedingRepository()

	err := repo.DeleteFeeding(context.Background(), "nonexistent")
	if err == nil {
		t.Error("expected error, got nil")
	}
}
