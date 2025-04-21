package controller_test

import (
	"bytes"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"hw_02/internal/domain/animal"
	"hw_02/internal/presentation/controller"
	"hw_02/testutils"
	"net/http"
	"net/http/httptest"
	"testing"
	"time"
)

func TestCreateAnimal(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("SaveAnimal", mock.Anything, mock.Anything).Return(nil)

	reqBody := `{
		"id": "1",
		"species": "Lion",
		"name": "Leo",
		"birth_date": "2020-06-01T00:00:00Z",
		"gender": "male"
	}`

	req := httptest.NewRequest(http.MethodPost, "/animals", bytes.NewBufferString(reqBody))
	req.Header.Set("Content-Type", "application/json")

	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusCreated, w.Code)
	assert.Contains(t, w.Body.String(), "animal created")
	mockService.AssertExpectations(t)
}

func TestGetAllAnimals(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	feedingTime := time.Date(
		2023,
		time.November,
		15,
		14,
		30,
		0,
		0,
		time.UTC,
	)
	a1, _ := animal.NewAnimal("1", "Lion", "Leo", feedingTime, animal.Male)
	a2, _ := animal.NewAnimal("2", "Tiger", "Интересно, кто-нибдуь это прочитает?", feedingTime, animal.Male)
	mockService.On("GetAllAnimals", mock.Anything).Return([]*animal.Animal{a1, a2}, nil)

	req := httptest.NewRequest(http.MethodGet, "/animals", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "Leo")
	assert.Contains(t, w.Body.String(), "Интересно, кто-нибдуь это прочитает?")
	mockService.AssertExpectations(t)
}

func TestGetAnimalByID(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	feedingTime := time.Date(
		2023,
		time.November,
		15,
		14,
		30,
		0,
		0,
		time.UTC,
	)
	a, _ := animal.NewAnimal("1", "Lion", "Leo", feedingTime, animal.Male)
	mockService.On("GetAnimalByID", mock.Anything, animal.AnimalID("1")).Return(a, nil)

	req := httptest.NewRequest(http.MethodGet, "/animals/1", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "Leo")
	mockService.AssertExpectations(t)
}

func TestDeleteAnimal(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("DeleteAnimal", mock.Anything, animal.AnimalID("1")).Return(nil)

	req := httptest.NewRequest(http.MethodDelete, "/animals/1", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "animal deleted")
	mockService.AssertExpectations(t)
}

func TestMakeSick(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("MakeSick", mock.Anything, animal.AnimalID("1")).Return(nil)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/sick", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "animal is sick")
	mockService.AssertExpectations(t)
}

func TestHealAnimal(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("Heal", mock.Anything, animal.AnimalID("1")).Return(nil)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/heal", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "animal heals")
	mockService.AssertExpectations(t)
}

func TestHealAnimal_Error(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("Heal", mock.Anything, animal.AnimalID("1")).Return(assert.AnError)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/heal", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusInternalServerError, w.Code)
	assert.Contains(t, w.Body.String(), "failed to heal")
	mockService.AssertExpectations(t)
}

func TestFeedAnimal(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	body := []byte(`{"food": "fish", "feeding_time": "Now"}`)

	mockService.On("Feed", mock.Anything, animal.AnimalID("1"), "fish", "Now").Return(nil)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/feed", bytes.NewBuffer(body))
	req.Header.Set("Content-Type", "application/json")
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "animal feeding")
	mockService.AssertExpectations(t)
}

func TestFeedAnimal_BadRequest(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	body := []byte(`{invalid json}`)
	req := httptest.NewRequest(http.MethodPut, "/animals/1/feed", bytes.NewBuffer(body))
	req.Header.Set("Content-Type", "application/json")

	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusBadRequest, w.Code)
	assert.Contains(t, w.Body.String(), "error")
}

func TestFeedAnimal_Error(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	body := []byte(`{"food": "meat", "feeding_time": "Now"}`)

	mockService.On("Feed", mock.Anything, animal.AnimalID("1"), "meat", "Now").Return(assert.AnError)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/feed", bytes.NewBuffer(body))
	req.Header.Set("Content-Type", "application/json")
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusInternalServerError, w.Code)
	assert.Contains(t, w.Body.String(), "error")
	mockService.AssertExpectations(t)
}

func TestMakeHungry(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("Hungry", mock.Anything, animal.AnimalID("1")).Return(nil)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/hungry", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "animal is hungry")
	mockService.AssertExpectations(t)
}

func TestMakeHungry_Error(t *testing.T) {
	mockService := new(testutils.MockAnimalService)
	c := controller.NewAnimalController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("Hungry", mock.Anything, animal.AnimalID("1")).Return(assert.AnError)

	req := httptest.NewRequest(http.MethodPut, "/animals/1/hungry", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusInternalServerError, w.Code)
	assert.Contains(t, w.Body.String(), "error")
	mockService.AssertExpectations(t)
}
