package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/feeding"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type FeedingController struct {
	service interfaces.FeedingServiceInterface
}

func NewFeedingController(service interfaces.FeedingServiceInterface) *FeedingController {
	return &FeedingController{service: service}
}

func (fc *FeedingController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/feeding")
	{
		group.POST("", fc.CreateFeeding)
		group.PUT("/:id", fc.EditFeeding)
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

type EditFeedingRequest struct {
	NewTime string `json:"new_time"`
	NewFood string `json:"new_food"`
}

// === Handlers ===

// CreateFeeding godoc
// @Summary Создать расписание кормления
// @Description Добавляет новое расписание кормления для животного
// @Tags feeding
// @Accept json
// @Produce json
// @Param feeding body controller.FeedingRequest true "Расписание кормления"
// @Success 201 {object} map[string]string
// @Failure 400 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /feeding [post]
func (fc *FeedingController) CreateFeeding(c *gin.Context) {
	var req FeedingRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	food, err := feeding.NewFoodType(req.Food)
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

// EditFeeding godoc
// @Summary Изменить расписание кормления
// @Description Обновляет время и тип еды для заданного расписания
// @Tags feeding
// @Accept json
// @Produce json
// @Param id path string true "Feeding ID"
// @Param feeding body controller.EditFeedingRequest true "Новое значение"
// @Success 204 {object} map[string]string
// @Failure 400 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /feeding/{id} [put]
func (fc *FeedingController) EditFeeding(c *gin.Context) {
	id := feeding.ScheduleID(c.Param("id"))

	var req EditFeedingRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	newFood, err := feeding.NewFoodType(req.NewFood)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid food type"})
		return
	}

	err = fc.service.EditFeeding(c, id, req.NewTime, newFood)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to edit feeding"})
		return
	}

	c.JSON(http.StatusNoContent, gin.H{"message": "feeding edited"})
}

// GetAllFeedings godoc
// @Summary Получить все расписания кормлений
// @Description Возвращает список всех кормлений
// @Tags feeding
// @Produce json
// @Success 200 {array} dto.FeedingResponse
// @Failure 500 {object} map[string]string
// @Router /feeding [get]
func (fc *FeedingController) GetAllFeedings(c *gin.Context) {
	feedings, err := fc.service.GetAllFeedings(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feedings"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingsToResponseList(feedings))
}

// GetFeedingByID godoc
// @Summary Получить расписание по ID
// @Description Возвращает одно расписание кормления
// @Tags feeding
// @Produce json
// @Param id path string true "Feeding ID"
// @Success 200 {object} dto.FeedingResponse
// @Failure 500 {object} map[string]string
// @Router /feeding/{id} [get]
func (fc *FeedingController) GetFeedingByID(c *gin.Context) {
	id := feeding.ScheduleID(c.Param("id"))

	f, err := fc.service.GetFeedingByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feeding"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingToResponse(f))
}

// GetFeedingByAnimalID godoc
// @Summary Получить расписание по животному
// @Description Возвращает расписание кормления по ID животного
// @Tags feeding
// @Produce json
// @Param animal_id path string true "Animal ID"
// @Success 200 {object} dto.FeedingResponse
// @Failure 500 {object} map[string]string
// @Router /feeding/animal/{animal_id} [get]
func (fc *FeedingController) GetFeedingByAnimalID(c *gin.Context) {
	animalID := animal.AnimalID(c.Param("animal_id"))

	f, err := fc.service.GetFeedingByAnimalID(c, animalID)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch feeding"})
		return
	}
	c.JSON(http.StatusOK, dto.MapFeedingToResponse(f))
}

// DeleteFeeding godoc
// @Summary Удалить расписание кормления
// @Description Удаляет кормление по ID
// @Tags feeding
// @Param id path string true "Feeding ID"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /feeding/{id} [delete]
func (fc *FeedingController) DeleteFeeding(c *gin.Context) {
	id := feeding.ScheduleID(c.Param("id"))

	err := fc.service.DeleteFeeding(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete feeding"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "feeding deleted"})
}
