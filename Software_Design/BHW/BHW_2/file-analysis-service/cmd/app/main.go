package main

import (
	"context"
	"file-analysis-service/internal/config"
	"file-analysis-service/internal/delivery/server"
	_ "file-analysis-service/internal/docs"
	fsClient "file-analysis-service/internal/repository/filestorage"
	pgRepo "file-analysis-service/internal/repository/pgstorage"
	wcClient "file-analysis-service/internal/repository/wordcloud"
	"file-analysis-service/internal/storage"
	"file-analysis-service/internal/usecase"
	"github.com/jackc/pgx/v5"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
)

// @title File Analysis API
// @version 1.0
// @description This is a service for analyzing text files and generating word cloud images.
// @host localhost:8082
// @BasePath /
func main() {
	cfg := config.Load()

	db, err := pgx.Connect(context.Background(), cfg.DatabaseURL)
	if err != nil {
		log.Fatal("DB connection failed:", err)
	}
	defer db.Close(context.Background())

	store := pgRepo.NewPgStorage(db)
	fileClient := fsClient.NewClient(cfg.FileStorageURL)
	wordCloudClient := wcClient.NewClient()
	imgSaver := storage.NewFSImageSaver(cfg.ImagesDir)

	inter := usecase.NewInteractor(store, fileClient, wordCloudClient, imgSaver)

	handler := server.NewHandler(inter)
	mux := server.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
