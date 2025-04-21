package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type EnclosureController struct {
	service interfaces.EnclosureServiceInterface
}

func NewEnclosureController(service interfaces.EnclosureServiceInterface) *EnclosureController {
	return &EnclosureController{service: service}
}

func (ec *EnclosureController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/enclosures")
	{
		group.POST("", ec.CreateEnclosure)
		group.GET("", ec.GetAllEnclosures)
		group.GET("/:id", ec.GetEnclosureByID)
		group.DELETE("/:id", ec.DeleteEnclosure)
		group.POST("/:id/add/:animal_id", ec.AddAnimalToEnclosure)
	}
}

// === Request DTO ===

type EnclosureRequest struct {
	ID            enclosure.EnclosureID `json:"id"`
	EnclosureType string                `json:"enclosure_type"`
	Capacity      int                   `json:"capacity"`
}

// === Handlers ===

// CreateEnclosure godoc
// @Summary Создать вольер
// @Description Добавляет новый вольер в систему
// @Tags enclosures
// @Accept json
// @Produce json
// @Param enclosure body controller.EnclosureRequest true "Вольер"
// @Success 201 {object} map[string]string
// @Failure 400 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /enclosures [post]
func (ec *EnclosureController) CreateEnclosure(c *gin.Context) {
	var req EnclosureRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	enclosureType, err := enclosure.NewEnclosureType(req.EnclosureType)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid enclosure type"})
		return
	}

	e, err := enclosure.NewEnclosure(req.ID, enclosureType, req.Capacity)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	err = ec.service.SaveEnclosure(c, e)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to save enclosure"})
		return
	}

	c.JSON(http.StatusCreated, gin.H{"message": "enclosure created"})
}

// GetAllEnclosures godoc
// @Summary Получить список всех вольеров
// @Description Возвращает все вольеры
// @Tags enclosures
// @Produce json
// @Success 200 {array} dto.EnclosureResponse
// @Failure 500 {object} map[string]string
// @Router /enclosures [get]
func (ec *EnclosureController) GetAllEnclosures(c *gin.Context) {
	enclosures, err := ec.service.GetAllEnclosure(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch enclosures"})
		return
	}
	c.JSON(http.StatusOK, dto.MapEnclosuresToResponseList(enclosures))
}

// GetEnclosureByID godoc
// @Summary Получить вольер по ID
// @Description Возвращает вольер по идентификатору
// @Tags enclosures
// @Produce json
// @Param id path string true "Enclosure ID"
// @Success 200 {object} dto.EnclosureResponse
// @Failure 500 {object} map[string]string
// @Router /enclosures/{id} [get]
func (ec *EnclosureController) GetEnclosureByID(c *gin.Context) {
	id := enclosure.EnclosureID(c.Param("id"))

	e, err := ec.service.GetEnclosureByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch enclosure"})
		return
	}
	c.JSON(http.StatusOK, dto.MapEnclosureToResponse(e))
}

// DeleteEnclosure godoc
// @Summary Удалить вольер
// @Description Удаляет вольер по ID
// @Tags enclosures
// @Param id path string true "Enclosure ID"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /enclosures/{id} [delete]
func (ec *EnclosureController) DeleteEnclosure(c *gin.Context) {
	id := enclosure.EnclosureID(c.Param("id"))

	err := ec.service.DeleteEnclosure(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete enclosure"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "enclosure deleted"})
}

// AddAnimalToEnclosure godoc
// @Summary Добавить животное в вольер
// @Description Привязывает животное к указанному вольеру
// @Tags enclosures
// @Param id path string true "Enclosure ID"
// @Param animal_id path string true "Animal ID"
// @Success 201 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /enclosures/{id}/add/{animal_id} [post]
func (ec *EnclosureController) AddAnimalToEnclosure(c *gin.Context) {
	enclosureID := enclosure.EnclosureID(c.Param("id"))
	animalID := animal.AnimalID(c.Param("animal_id"))

	err := ec.service.AddAnimalToEnclosure(c, enclosureID, animalID)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusCreated, gin.H{"message": "animal added to enclosure"})
}
