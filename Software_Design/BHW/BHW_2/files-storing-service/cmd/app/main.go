package main

import (
	"context"
	"files-storing-service/internal/config"
	"files-storing-service/internal/delivery/server"
	_ "files-storing-service/internal/docs"
	pgRepo "files-storing-service/internal/repository/pgstorage"
	fSaver "files-storing-service/internal/storage"
	"files-storing-service/internal/usecase"
	"github.com/jackc/pgx/v5"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
)

// @title Files Storing API
// @version 1.0
// @description This is a service for storing files and getting their texts.
// @host localhost:8081
// @BasePath /
func main() {
	cfg := config.Load()

	db, err := pgx.Connect(context.Background(), cfg.DatabaseURL)
	if err != nil {
		log.Fatal("DB connection failed:", err)
	}
	defer db.Close(context.Background())

	storage := pgRepo.NewPgStorage(db)
	fileSaver := fSaver.NewFileSaver(cfg.BaseDir)

	inter := usecase.NewIntercator(storage, fileSaver)

	handler := server.NewHandler(inter)
	mux := server.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
