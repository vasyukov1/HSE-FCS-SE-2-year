package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/domain/enclosure"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type EnclosureController struct {
	service *services.EnclosureService
}

func NewEnclosureController(service *services.EnclosureService) *EnclosureController {
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

func (ec *EnclosureController) GetAllEnclosures(c *gin.Context) {
	enclosures, err := ec.service.GetAllEnclosure(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch enclosures"})
		return
	}
	c.JSON(http.StatusOK, dto.MapEnclosuresToResponseList(enclosures))
}

func (ec *EnclosureController) GetEnclosureByID(c *gin.Context) {
	id := enclosure.EnclosureID(c.Param("id"))

	e, err := ec.service.GetEnclosureByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch enclosure"})
		return
	}
	c.JSON(http.StatusOK, dto.MapEnclosureToResponse(e))
}

func (ec *EnclosureController) DeleteEnclosure(c *gin.Context) {
	id := enclosure.EnclosureID(c.Param("id"))

	err := ec.service.DeleteEnclosure(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete enclosure"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "enclosure deleted"})
}

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
