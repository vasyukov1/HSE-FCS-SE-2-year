package main

import (
	"github.com/gin-gonic/gin"
	"hw_02/internal/application/services"
	"hw_02/internal/infrastructure/storage"
	"hw_02/internal/presentation/controller"
)

func main() {
	r := gin.Default()

	animalRepo := storage.NewAnimalRepository()
	animalService := services.NewAnimalService(animalRepo)
	animalController := controller.NewAnimalController(animalService)
	animalController.RegisterRoutes(r)

	enclosureRepo := storage.NewEnclosureRepository()
	enclosureService := services.NewEnclosureService(enclosureRepo, animalRepo)
	enclosureController := controller.NewEnclosureController(enclosureService)
	enclosureController.RegisterRoutes(r)

	_ = r.Run(":8080")
}
