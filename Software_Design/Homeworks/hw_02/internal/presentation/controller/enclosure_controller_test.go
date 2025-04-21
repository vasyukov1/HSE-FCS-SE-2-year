package controller_test

import (
	"bytes"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/presentation/controller"
	"hw_02/testutils"
	"net/http"
	"net/http/httptest"
	"testing"
)

func TestCreateEnclosure(t *testing.T) {
	mockService := new(testutils.MockEnclosureService)
	c := controller.NewEnclosureController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("SaveEnclosure", mock.Anything, mock.Anything).Return(nil)

	reqBody := `{
		"id": "e1",
		"enclosure_type": "predator",
		"capacity": 5
	}`

	req := httptest.NewRequest(http.MethodPost, "/enclosures", bytes.NewBufferString(reqBody))
	req.Header.Set("Content-Type", "application/json")

	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusCreated, w.Code)
	assert.Contains(t, w.Body.String(), "enclosure created")
	mockService.AssertExpectations(t)
}

func TestGetAllEnclosures(t *testing.T) {
	mockService := new(testutils.MockEnclosureService)
	c := controller.NewEnclosureController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	e1, _ := enclosure.NewEnclosure("e1", enclosure.Bird, 5)
	e2, _ := enclosure.NewEnclosure("e2", enclosure.Aquarium, 3)
	mockService.On("GetAllEnclosure", mock.Anything).Return([]*enclosure.Enclosure{e1, e2}, nil)

	req := httptest.NewRequest(http.MethodGet, "/enclosures", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "e1")
	assert.Contains(t, w.Body.String(), "e2")
	mockService.AssertExpectations(t)
}

func TestGetEnclosureByID(t *testing.T) {
	mockService := new(testutils.MockEnclosureService)
	c := controller.NewEnclosureController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	e, _ := enclosure.NewEnclosure("e1", enclosure.Predator, 5)
	mockService.On("GetEnclosureByID", mock.Anything, enclosure.EnclosureID("e1")).Return(e, nil)

	req := httptest.NewRequest(http.MethodGet, "/enclosures/e1", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "e1")
	mockService.AssertExpectations(t)
}

func TestDeleteEnclosure(t *testing.T) {
	mockService := new(testutils.MockEnclosureService)
	c := controller.NewEnclosureController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("DeleteEnclosure", mock.Anything, enclosure.EnclosureID("e1")).Return(nil)

	req := httptest.NewRequest(http.MethodDelete, "/enclosures/e1", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusOK, w.Code)
	assert.Contains(t, w.Body.String(), "enclosure deleted")
	mockService.AssertExpectations(t)
}

func TestAddAnimalToEnclosure(t *testing.T) {
	mockService := new(testutils.MockEnclosureService)
	c := controller.NewEnclosureController(mockService)

	r := gin.Default()
	c.RegisterRoutes(r)

	mockService.On("AddAnimalToEnclosure", mock.Anything, enclosure.EnclosureID("e1"), animal.AnimalID("a1")).Return(nil)

	req := httptest.NewRequest(http.MethodPost, "/enclosures/e1/add/a1", nil)
	w := httptest.NewRecorder()
	r.ServeHTTP(w, req)

	assert.Equal(t, http.StatusCreated, w.Code)
	assert.Contains(t, w.Body.String(), "animal added to enclosure")
	mockService.AssertExpectations(t)
}
