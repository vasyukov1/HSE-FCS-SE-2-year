package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/animal"
	"hw_02/internal/presentation/dto"
	"net/http"
	"time"
)

type AnimalController struct {
	service *services.AnimalService
}

func NewAnimalController(service *services.AnimalService) *AnimalController {
	return &AnimalController{service: service}
}

func (ac *AnimalController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/animals")
	{
		group.POST("", ac.CreateAnimal)
		group.GET("", ac.GetAllAnimals)
		group.GET("/:id", ac.GetAnimalByID)
		group.DELETE("/:id", ac.DeleteAnimal)
	}
}

// === Request DTO ===

type AnimalRequest struct {
	ID        animal.AnimalID `json:"id"`
	Species   string          `json:"species"`
	Name      string          `json:"name"`
	BirthDate time.Time       `json:"birth_date"`
	Gender    string          `json:"gender"`
	Food      string          `json:"food"`
}

// === Handlers ===

func (ac *AnimalController) CreateAnimal(c *gin.Context) {
	var req AnimalRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	gender, err := animal.NewGender(req.Gender)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid gender"})
		return
	}

	food, err := animal.NewFoodType(req.Food)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid food type"})
		return
	}

	a, err := animal.NewAnimal(req.ID, req.Species, req.Name, req.BirthDate, gender, food)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	err = ac.service.SaveAnimal(c, a)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to save animal"})
		return
	}

	c.JSON(http.StatusCreated, gin.H{"message": "animal created"})
}

func (ac *AnimalController) GetAllAnimals(c *gin.Context) {
	animals, err := ac.service.GetAllAnimals(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch animals"})
		return
	}
	c.JSON(http.StatusOK, dto.MapAnimalsToResponseList(animals))
}

func (ac *AnimalController) GetAnimalByID(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	a, err := ac.service.GetAnimalByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch animal"})
		return
	}
	c.JSON(http.StatusOK, dto.MapAnimalToResponse(a))
}

func (ac *AnimalController) DeleteAnimal(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	err := ac.service.DeleteAnimal(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete animal"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal deleted"})
}
