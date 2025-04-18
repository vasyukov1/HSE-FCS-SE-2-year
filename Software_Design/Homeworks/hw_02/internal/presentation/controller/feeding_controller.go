package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type FeedingController struct {
	service *services.FeedingService
}

func NewFeedingController(service *services.FeedingService) *FeedingController {
	return &FeedingController{service: service}
}

func (fc *FeedingController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/feeding")
	{
		group.POST("", fc.CreateFeeding)
		group.GET("", fc.GetAllFeedings)
		group.GET("/:id", fc.GetFeedingByID)
		group.GET("/animal/:animal_id", fc.GetFeedingByAnimalID)
		group.DELETE("/:id", fc.DeleteFeeding)
	}
}

// === Request DTO ===

type FeedingRequest struct {
	ID       feeding.ScheduleID `json:"id"`
	AnimalID string             `json:"animal_id"`
	Time     string             `json:"time"`
	Food     string             `json:"food"`
}

// === Handlers ===

func (fc *FeedingController) CreateFeeding(c *gin.Context) {
	var req FeedingRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	food, err := animal.NewFoodType(req.Food)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid food type"})
		return
	}

	f, err := feeding.NewFeeding(req.ID, req.AnimalID, req.Time, food)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	err = fc.service.SaveFeeding(c, f)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to save feeding"})
		return
	}

	c.JSON(http.StatusCreated, gin.H{"message": "feeding created"})
}

func (fc *FeedingController) GetAllFeedings(c *gin.Context) {
	feedings, err := fc.service.GetAllFeedings(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feedings"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingsToResponseList(feedings))
}

func (fc *FeedingController) GetFeedingByID(c *gin.Context) {
	id := feeding.ScheduleID(c.Param("id"))

	f, err := fc.service.GetFeedingByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feeding"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingToResponse(f))
}

func (fc *FeedingController) GetFeedingByAnimalID(c *gin.Context) {
	animalID := animal.AnimalID(c.Param("animal_id"))

	f, err := fc.service.GetFeedingByAnimalID(c, animalID)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feeding"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingToResponse(f))
}

func (fc *FeedingController) DeleteFeeding(c *gin.Context) {
	id := feeding.ScheduleID(c.Param("id"))

	err := fc.service.DeleteFeeding(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete feeding"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "feeding deleted"})
}
