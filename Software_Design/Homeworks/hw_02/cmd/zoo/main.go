// @title Zoo API
// @version 1.0
// @description This is the Swagger documentation for the Zoo project
// @host localhost:8080
// @BasePath /

package main

import (
	"github.com/gin-gonic/gin"
	swaggerFiles "github.com/swaggo/files"
	ginSwagger "github.com/swaggo/gin-swagger"
	_ "hw_02/docs"
	"hw_02/internal/application/services"
	"hw_02/internal/domain/events"
	"hw_02/internal/infrastructure/storage"
	"hw_02/internal/presentation/controller"
)

func main() {
	r := gin.Default()

	animalRepo := storage.NewAnimalRepository()
	enclosureRepo := storage.NewEnclosureRepository()
	feedingRepo := storage.NewFeedingRepository()
	eventDispatcher := &events.ConsoleDispatcher{}

	animalService := services.NewAnimalService(animalRepo, feedingRepo)
	animalController := controller.NewAnimalController(animalService)
	animalController.RegisterRoutes(r)

	enclosureService := services.NewEnclosureService(enclosureRepo, animalRepo, eventDispatcher)
	enclosureController := controller.NewEnclosureController(enclosureService)
	enclosureController.RegisterRoutes(r)

	feedingService := services.NewFeedingService(feedingRepo, animalRepo, eventDispatcher)
	feedingController := controller.NewFeedingController(feedingService)
	feedingController.RegisterRoutes(r)

	statsService := services.NewStatsService(animalRepo, enclosureRepo, feedingRepo)
	statsController := controller.NewStatsController(statsService)
	statsController.RegisterRoutes(r)

	r.GET("/swagger/*any", ginSwagger.WrapHandler(swaggerFiles.Handler))
	_ = r.Run(":8080")
}
