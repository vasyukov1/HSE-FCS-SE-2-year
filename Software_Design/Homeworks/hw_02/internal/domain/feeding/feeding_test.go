package feeding_test

import (
	"hw_02/internal/domain/feeding"
	"testing"
)

func TestNewFeeding_ValidData(t *testing.T) {
	feed, err := feeding.NewFeeding("s1", "a1", "12:00", "Meat")
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}

	if feed.ID() != "s1" {
		t.Errorf("expected ID s1, got %s", feed.ID())
	}

	if feed.AnimalID() != "a1" {
		t.Errorf("expected animal ID a1, got %s", feed.AnimalID())
	}

	if feed.Time() != "12:00" {
		t.Errorf("expected time 12:00, got %s", feed.Time())
	}

	if feed.Food() != "Meat" {
		t.Errorf("expected food type Meat, got %s", feed.Food())
	}
}

func TestNewFeeding_InvalidFood(t *testing.T) {
	_, err := feeding.NewFeeding("s2", "a2", "14:00", "")
	if err == nil {
		t.Error("expected error for empty food type, got nil")
	}
}

func TestFeeding_EditTime(t *testing.T) {
	feed, _ := feeding.NewFeeding("s3", "a3", "10:00", "Grass")
	feed.EditTime("16:30")

	if feed.Time() != "16:30" {
		t.Errorf("expected updated time 16:30, got %s", feed.Time())
	}
}

func TestFeeding_EditFood(t *testing.T) {
	feed, _ := feeding.NewFeeding("s4", "a4", "09:00", "Fish")
	feed.EditFood("Vegetables")

	if feed.Food() != "Vegetables" {
		t.Errorf("expected updated food type Vegetables, got %s", feed.Food())
	}
}
