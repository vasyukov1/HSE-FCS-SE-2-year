package controller_test

import (
	"bytes"
	"encoding/json"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"github.com/stretchr/testify/require"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"hw_02/internal/presentation/controller"
	"hw_02/testutils"
	"net/http"
	"net/http/httptest"
	"testing"
)

func TestCreateFeeding(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	reqBody := controller.FeedingRequest{
		ID:       "f1",
		AnimalID: "a1",
		Time:     "12:00",
		Food:     "meat",
	}

	jsonBody, _ := json.Marshal(reqBody)
	req, _ := http.NewRequest(http.MethodPost, "/feeding", bytes.NewBuffer(jsonBody))
	req.Header.Set("Content-Type", "application/json")
	w := httptest.NewRecorder()

	expectedFeeding, _ := feeding.NewFeeding("f1", "a1", "12:00", feeding.Meat)
	mockService.On("SaveFeeding", mock.Anything, expectedFeeding).Return(nil)

	router.ServeHTTP(w, req)

	assert.Equal(t, http.StatusCreated, w.Code)
	assert.Contains(t, w.Body.String(), "feeding created")
}

func TestFeedingController_EditFeeding(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	body := `{"new_time": "12:00", "new_food": "meat"}`
	req, _ := http.NewRequest(http.MethodPut, "/feeding/abc123", bytes.NewBuffer([]byte(body)))
	req.Header.Set("Content-Type", "application/json")
	w := httptest.NewRecorder()

	mockService.On("EditFeeding", mock.Anything, feeding.ScheduleID("abc123"), "12:00", feeding.Meat).Return(nil)

	router.ServeHTTP(w, req)

	require.Equal(t, http.StatusNoContent, w.Code)
	mockService.AssertExpectations(t)
}

func TestFeedingController_GetAllFeedings(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	expected := []*feeding.Feeding{
		testutils.MustFeeding("1", "a1", "10:00", feeding.Meat),
		testutils.MustFeeding("2", "a2", "11:00", feeding.Fish),
	}

	mockService.On("GetAllFeedings", mock.Anything).Return(expected, nil)

	req, _ := http.NewRequest(http.MethodGet, "/feeding", nil)
	w := httptest.NewRecorder()
	router.ServeHTTP(w, req)

	require.Equal(t, http.StatusOK, w.Code)
	mockService.AssertExpectations(t)
}

func TestFeedingController_GetFeedingByID(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	f := testutils.MustFeeding("f123", "a1", "12:30", feeding.Meat)
	mockService.On("GetFeedingByID", mock.Anything, feeding.ScheduleID("f123")).Return(f, nil)

	req, _ := http.NewRequest(http.MethodGet, "/feeding/f123", nil)
	w := httptest.NewRecorder()
	router.ServeHTTP(w, req)

	require.Equal(t, http.StatusOK, w.Code)
	mockService.AssertExpectations(t)
}

func TestFeedingController_GetFeedingByAnimalID(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	f := testutils.MustFeeding("f1", "a1", "13:00", feeding.Fish)
	mockService.On("GetFeedingByAnimalID", mock.Anything, animal.AnimalID("a1")).Return(f, nil)

	req, _ := http.NewRequest(http.MethodGet, "/feeding/animal/a1", nil)
	w := httptest.NewRecorder()
	router.ServeHTTP(w, req)

	require.Equal(t, http.StatusOK, w.Code)
	mockService.AssertExpectations(t)
}

func TestFeedingController_DeleteFeeding(t *testing.T) {
	gin.SetMode(gin.TestMode)

	mockService := new(testutils.MockFeedingService)
	c := controller.NewFeedingController(mockService)

	router := gin.Default()
	c.RegisterRoutes(router)

	mockService.On("DeleteFeeding", mock.Anything, feeding.ScheduleID("f1")).Return(nil)

	req, _ := http.NewRequest(http.MethodDelete, "/feeding/f1", nil)
	w := httptest.NewRecorder()
	router.ServeHTTP(w, req)

	require.Equal(t, http.StatusOK, w.Code)
	mockService.AssertExpectations(t)
}
