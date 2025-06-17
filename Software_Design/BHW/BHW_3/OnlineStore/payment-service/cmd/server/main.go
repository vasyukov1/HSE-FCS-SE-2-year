package main

import (
	"context"
	"github.com/jackc/pgx/v5"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
	"payment-service/internal/config"
	"payment-service/internal/delivery"
	_ "payment-service/internal/docs"
	"payment-service/internal/infrastructure"
	"payment-service/internal/repository"
)

// @title Payment Service
// @version 1.0
// @description Payment Service is for creating, top up and watching accounts.
// @BasePath /
func main() {
	cfg := config.Load()

	db, err := pgx.Connect(context.Background(), cfg.DatabaseURL)
	if err != nil {
		log.Fatal("DB connection failed:", err)
	}
	defer db.Close(context.Background())

	storage := repository.NewPgStorage(db)
	server := infrastructure.NewServer(storage)

	handler := delivery.NewHandler(server)
	mux := delivery.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on port %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
