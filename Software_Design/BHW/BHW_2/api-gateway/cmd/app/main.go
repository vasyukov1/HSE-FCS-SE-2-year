package main

import (
	"api-gateway/internal/config"
	"api-gateway/internal/delivery/server"
	_ "api-gateway/internal/docs"
	"api-gateway/internal/infrastructure"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
)

// @title API Gateway
// @version 1.0
// @description Gateway for file upload, download, analysis and getting word cloud
// @BasePath /
func main() {
	cfg := config.Load()

	analysisService := infrastructure.NewAnalysisClient(cfg.AnalysisService)
	fileService := infrastructure.NewFileClient(cfg.FileService)

	handler := server.NewHandler(analysisService, fileService)
	mux := server.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
