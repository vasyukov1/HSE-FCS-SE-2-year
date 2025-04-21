package controller

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/services"
	"hw_02/internal/presentation/dto"
	"net/http"
)

type StatsController struct {
	service *services.StatsService
}

func NewStatsController(service *services.StatsService) *StatsController {
	return &StatsController{service: service}
}

func (sc *StatsController) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/stats")
	{
		group.GET("", sc.GetAllStats)
		group.GET("/animals", sc.GetAnimalsStats)
	}
}

// GetAllStats godoc
// @Summary Заглушка статистики
// @Description Просто возвращает сообщение-заглушку
// @Tags stats
// @Produce json
// @Success 200 {object} map[string]string
// @Router /stats [get]
func (sc *StatsController) GetAllStats(c *gin.Context) {
	animals := len(sc.service.GetAnimals(c))
	enclosures := len(sc.service.GetEnclosures(c))
	feedings := len(sc.service.GetFeedings(c))
	c.JSON(http.StatusOK, dto.MapGeneralStatsToResponse(animals, enclosures, feedings))
}

// GetAnimalsStats godoc
// @Summary Получить статистику по животным
// @Description Возвращает сводную информацию о животных: здоровье, пол, наличие вольера
// @Tags stats
// @Produce json
// @Success 200 {object} dto.StatsResponse
// @Failure 500 {object} map[string]string
// @Router /stats/animals [get]
func (sc *StatsController) GetAnimalsStats(c *gin.Context) {
	animals := sc.service.GetAnimals(c)

	var animalIDs []string
	var healthyAnimals []string
	var seekAnimals []string
	var male []string
	var female []string
	var withEnclosure []string
	var withoutEnclosure []string

	for _, a := range animals {
		id := string(a.ID())
		animalIDs = append(animalIDs, id)

		if a.IsHealthy() {
			healthyAnimals = append(healthyAnimals, id)
		} else {
			seekAnimals = append(seekAnimals, id)
		}

		if a.IsMale() {
			male = append(male, id)
		} else {
			female = append(female, id)
		}

		if a.EnclosureId() != "" {
			withEnclosure = append(withEnclosure, id)
		} else {
			withoutEnclosure = append(withoutEnclosure, id)
		}
	}

	total := len(animals)
	countHealthy := len(healthyAnimals)
	countSeek := len(seekAnimals)
	countMale := len(male)
	countFemale := len(female)
	countWithEnclosure := len(withEnclosure)
	countWithoutEnclosure := len(withoutEnclosure)

	c.JSON(http.StatusOK, dto.MapStatsToResponse(
		total, countHealthy, countSeek, countMale, countFemale, countWithEnclosure, countWithoutEnclosure,
		animalIDs, healthyAnimals, seekAnimals, male, female, withEnclosure, withoutEnclosure))
}
