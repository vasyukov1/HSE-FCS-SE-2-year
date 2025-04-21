package testutils

import (
	"context"
	"github.com/stretchr/testify/mock"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/domain/feeding"
	"time"
)

// === MockAnimalRepo ===

type MockAnimalRepo struct {
	mock.Mock
}

func (m *MockAnimalRepo) SaveAnimal(ctx context.Context, a *animal.Animal) error {
	args := m.Called(ctx, a)
	return args.Error(0)
}

func (m *MockAnimalRepo) GetAnimalByID(ctx context.Context, id animal.AnimalID) (*animal.Animal, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*animal.Animal), args.Error(1)
}

func (m *MockAnimalRepo) GetAllAnimals(ctx context.Context) ([]*animal.Animal, error) {
	args := m.Called(ctx)
	return args.Get(0).([]*animal.Animal), args.Error(1)
}

func (m *MockAnimalRepo) DeleteAnimal(ctx context.Context, id animal.AnimalID) error {
	args := m.Called(ctx, id)
	return args.Error(0)
}

func MustAnimal(id, gender string) *animal.Animal {
	g, _ := animal.NewGender(gender)
	a, _ := animal.NewAnimal(animal.AnimalID(id), "Tiger", "Simba", time.Now(), g)
	return a
}

// === MockFeedingRepo ===

type MockFeedingRepo struct {
	mock.Mock
}

func (m *MockFeedingRepo) SaveFeeding(ctx context.Context, f *feeding.Feeding) error {
	args := m.Called(ctx, f)
	return args.Error(0)
}

func (m *MockFeedingRepo) GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error) {
	args := m.Called(ctx)
	return args.Get(0).([]*feeding.Feeding), args.Error(1)
}

func (m *MockFeedingRepo) GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*feeding.Feeding), args.Error(1)
}

func (m *MockFeedingRepo) GetFeedingByAnimalID(ctx context.Context, id animal.AnimalID) (*feeding.Feeding, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*feeding.Feeding), args.Error(1)
}

func (m *MockFeedingRepo) DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error {
	args := m.Called(ctx, id)
	return args.Error(0)
}

func MustFeeding(id, animalID, timeStr string, food feeding.FoodType) *feeding.Feeding {
	f, _ := feeding.NewFeeding(feeding.ScheduleID(id), animalID, timeStr, food)
	return f
}

// === MockEnclosureRepo ===

type MockEnclosureRepo struct{ mock.Mock }

func (m *MockEnclosureRepo) SaveEnclosure(ctx context.Context, e *enclosure.Enclosure) error {
	args := m.Called(ctx, e)
	return args.Error(0)
}
func (m *MockEnclosureRepo) GetAllEnclosure(ctx context.Context) ([]*enclosure.Enclosure, error) {
	args := m.Called(ctx)
	return args.Get(0).([]*enclosure.Enclosure), args.Error(1)
}
func (m *MockEnclosureRepo) GetEnclosureByID(ctx context.Context, id enclosure.EnclosureID) (*enclosure.Enclosure, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*enclosure.Enclosure), args.Error(1)
}
func (m *MockEnclosureRepo) DeleteEnclosure(ctx context.Context, id enclosure.EnclosureID) error {
	args := m.Called(ctx, id)
	return args.Error(0)
}
func (m *MockEnclosureRepo) AddAnimalToEnclosure(ctx context.Context, id enclosure.EnclosureID, animalID animal.AnimalID) error {
	args := m.Called(ctx, id, animalID)
	return args.Error(0)
}
func (m *MockEnclosureRepo) RemoveAnimalFromEnclosure(ctx context.Context, id enclosure.EnclosureID, animalID animal.AnimalID) error {
	args := m.Called(ctx, id, animalID)
	return args.Error(0)
}

func MustEnclosure(id enclosure.EnclosureID, enclosureType enclosure.EnclosureType, capacity int) *enclosure.Enclosure {
	e, _ := enclosure.NewEnclosure(id, enclosureType, capacity)
	return e
}

// === MockDispatcher ===

type MockDispatcher struct {
	mock.Mock
}

func (m *MockDispatcher) Dispatch(event interface{}) {
	m.Called(event)
}

// === MockAnimalService ===

type MockAnimalService struct {
	mock.Mock
}

func (m *MockAnimalService) SaveAnimal(c context.Context, a *animal.Animal) error {
	args := m.Called(c, a)
	return args.Error(0)
}
func (m *MockAnimalService) GetAllAnimals(c context.Context) ([]*animal.Animal, error) {
	args := m.Called(c)
	return args.Get(0).([]*animal.Animal), args.Error(1)
}
func (m *MockAnimalService) GetAnimalByID(c context.Context, id animal.AnimalID) (*animal.Animal, error) {
	args := m.Called(c, id)
	return args.Get(0).(*animal.Animal), args.Error(1)
}
func (m *MockAnimalService) DeleteAnimal(c context.Context, id animal.AnimalID) error {
	args := m.Called(c, id)
	return args.Error(0)
}
func (m *MockAnimalService) MakeSick(c context.Context, id animal.AnimalID) error {
	args := m.Called(c, id)
	return args.Error(0)
}
func (m *MockAnimalService) Heal(c context.Context, id animal.AnimalID) error {
	args := m.Called(c, id)
	return args.Error(0)
}
func (m *MockAnimalService) Feed(c context.Context, id animal.AnimalID, food string, feedingTime string) error {
	args := m.Called(c, id, food, feedingTime)
	return args.Error(0)
}
func (m *MockAnimalService) Hungry(c context.Context, id animal.AnimalID) error {
	args := m.Called(c, id)
	return args.Error(0)
}

// === MockEnclosureService ===

type MockEnclosureService struct {
	mock.Mock
}

func (m *MockEnclosureService) SaveEnclosure(ctx context.Context, e *enclosure.Enclosure) error {
	args := m.Called(ctx, e)
	return args.Error(0)
}
func (m *MockEnclosureService) GetAllEnclosure(ctx context.Context) ([]*enclosure.Enclosure, error) {
	args := m.Called(ctx)
	return args.Get(0).([]*enclosure.Enclosure), args.Error(1)
}
func (m *MockEnclosureService) GetEnclosureByID(ctx context.Context, id enclosure.EnclosureID) (*enclosure.Enclosure, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*enclosure.Enclosure), args.Error(1)
}
func (m *MockEnclosureService) DeleteEnclosure(ctx context.Context, id enclosure.EnclosureID) error {
	args := m.Called(ctx, id)
	return args.Error(0)
}
func (m *MockEnclosureService) AddAnimalToEnclosure(ctx context.Context, id enclosure.EnclosureID, animalID animal.AnimalID) error {
	args := m.Called(ctx, id, animalID)
	return args.Error(0)
}

// === MockFeedingService ===

type MockFeedingService struct {
	mock.Mock
}

func (m *MockFeedingService) SaveFeeding(ctx context.Context, f *feeding.Feeding) error {
	args := m.Called(ctx, f)
	return args.Error(0)
}
func (m *MockFeedingService) EditFeeding(ctx context.Context, id feeding.ScheduleID, newTime string, newFood feeding.FoodType) error {
	args := m.Called(ctx, id, newTime, newFood)
	return args.Error(0)
}
func (m *MockFeedingService) DeleteFeeding(ctx context.Context, id feeding.ScheduleID) error {
	args := m.Called(ctx, id)
	return args.Error(0)
}
func (m *MockFeedingService) GetAllFeedings(ctx context.Context) ([]*feeding.Feeding, error) {
	args := m.Called(ctx)
	return args.Get(0).([]*feeding.Feeding), args.Error(1)
}
func (m *MockFeedingService) GetFeedingByID(ctx context.Context, id feeding.ScheduleID) (*feeding.Feeding, error) {
	args := m.Called(ctx, id)
	return args.Get(0).(*feeding.Feeding), args.Error(1)
}
func (m *MockFeedingService) GetFeedingByAnimalID(ctx context.Context, animalID animal.AnimalID) (*feeding.Feeding, error) {
	args := m.Called(ctx, animalID)
	return args.Get(0).(*feeding.Feeding), args.Error(1)
}
