package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/interfaces"
	"hw_02/internal/domain/animal"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type AnimalController struct {
	service interfaces.AnimalServiceInterface
}

func NewAnimalController(service interfaces.AnimalServiceInterface) *AnimalController {
	return &AnimalController{service: service}
}

func (ac *AnimalController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/animals")
	{
		group.POST("", ac.CreateAnimal)
		group.GET("", ac.GetAllAnimals)
		group.GET("/:id", ac.GetAnimalByID)
		group.DELETE("/:id", ac.DeleteAnimal)
		group.PUT("/:id/sick", ac.MakeSick)
		group.PUT("/:id/heal", ac.Heal)
		group.PUT("/:id/feed", ac.Feed)
		group.PUT("/:id/hungry", ac.Hungry)
	}
}

// === Handlers ===

// CreateAnimal godoc
// @Summary Создать животное
// @Description Добавляет новое животное
// @Tags animals
// @Accept json
// @Produce json
// @Param animal body dto.AnimalRequest true "Животное"
// @Success 201 {object} map[string]string
// @Failure 400 {object} map[string]string
// @Router /animals [post]
func (ac *AnimalController) CreateAnimal(c *gin.Context) {
	var req dto.AnimalRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	gender, err := animal.NewGender(req.Gender)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid gender"})
		return
	}

	a, err := animal.NewAnimal(req.ID, req.Species, req.Name, req.BirthDate, gender)
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

// GetAllAnimals godoc
// @Summary Получить список животных
// @Description Возвращает всех зарегистрированных животных
// @Tags animals
// @Produce json
// @Success 200 {array} dto.AnimalResponse
// @Failure 500 {object} map[string]string
// @Router /animals [get]
func (ac *AnimalController) GetAllAnimals(c *gin.Context) {
	animals, err := ac.service.GetAllAnimals(c)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch animals"})
		return
	}
	c.JSON(http.StatusOK, dto.MapAnimalsToResponseList(animals))
}

// GetAnimalByID godoc
// @Summary Получить животное по ID
// @Description Возвращает одно животное по идентификатору
// @Tags animals
// @Produce json
// @Param id path string true "Animal ID"
// @Success 200 {object} dto.AnimalResponse
// @Failure 500 {object} map[string]string
// @Router /animals/{id} [get]
func (ac *AnimalController) GetAnimalByID(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	a, err := ac.service.GetAnimalByID(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to fetch animal"})
		return
	}
	c.JSON(http.StatusOK, dto.MapAnimalToResponse(a))
}

// DeleteAnimal godoc
// @Summary Удалить животное
// @Description Удаляет животное по ID
// @Tags animals
// @Param id path string true "Animal ID"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /animals/{id} [delete]
func (ac *AnimalController) DeleteAnimal(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	err := ac.service.DeleteAnimal(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to delete animal"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal deleted"})
}

// MakeSick godoc
// @Summary Отметить животное как больное
// @Description Изменяет статус здоровья животного на "sick" по его ID
// @Tags animals
// @Param id path string true "ID животного"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /animals/{id}/seek [put]
func (ac *AnimalController) MakeSick(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	err := ac.service.MakeSick(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to make sick"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal is sick"})
}

// Heal godoc
// @Summary Вылечить животное
// @Description Изменяет статус здоровья животного на "healthy" по его ID
// @Tags animals
// @Param id path string true "ID животного"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /animals/{id}/heal [put]
func (ac *AnimalController) Heal(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	err := ac.service.Heal(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "failed to heal"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal heals"})
}

// Feed godoc
// @Summary Покормить животное
// @Description Кормит животное определённой едой и сохраняет время кормления
// @Tags animals
// @Accept json
// @Produce json
// @Param id path string true "ID животного"
// @Param feed body dto.FeedRequest true "Информация о кормлении"
// @Success 200 {object} map[string]string
// @Failure 400 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /animals/{id}/feed [put]
func (ac *AnimalController) Feed(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	var req dto.FeedRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	err := ac.service.Feed(c, id, string(req.Food), req.FeedingTime)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal feeding"})
}

// Hungry godoc
// @Summary Сделать животное голодным
// @Description Помечает животное как голодное и очищает последнее время кормления
// @Tags animals
// @Param id path string true "ID животного"
// @Success 200 {object} map[string]string
// @Failure 500 {object} map[string]string
// @Router /animals/{id}/hungry [put]
func (ac *AnimalController) Hungry(c *gin.Context) {
	id := animal.AnimalID(c.Param("id"))

	err := ac.service.Hungry(c, id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message": "animal is hungry"})
}
