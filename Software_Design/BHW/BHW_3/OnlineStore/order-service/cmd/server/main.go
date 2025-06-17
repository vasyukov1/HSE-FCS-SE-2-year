package main

import (
	"context"
	"github.com/jackc/pgx/v5"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
	_ "order-service/internal/docs"
	"order-service/internal/kafka"
	"order-service/internal/payment"

	"order-service/internal/config"
	"order-service/internal/delivery"
	"order-service/internal/infrastructure"
	"order-service/internal/repository"
)

// @title Order Service
// @version 1.0
// @description Order Service is for creating and watching orders.
// @BasePath /
func main() {
	cfg := config.Load()

	db, err := pgx.Connect(context.Background(), cfg.DatabaseURL)
	if err != nil {
		log.Fatal("DB connection failed:", err)
	}
	defer db.Close(context.Background())

	storage := repository.NewPgStorage(db)

	var producer *kafka.Producer
	if len(cfg.Kafka.Brokers) > 0 && cfg.Kafka.Topic != "" {
		producer = kafka.NewProducer(cfg.Kafka.Brokers, cfg.Kafka.Topic)
		defer producer.Close()
	}

	paymentClient := payment.NewPaymentClient(cfg.PaymentServiceURL)
	server := infrastructure.NewServer(storage, producer, paymentClient)

	handler := delivery.NewHandler(server)
	mux := delivery.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on port %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
