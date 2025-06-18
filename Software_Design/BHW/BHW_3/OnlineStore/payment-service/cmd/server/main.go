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
	"payment-service/internal/kafka"
	"payment-service/internal/repository"
	"time"
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

	inboxRepo := repository.NewInboxRepo(db)
	outboxRepo := repository.NewOutboxRepo(db)

	producer := kafka.NewProducer(cfg.Kafka.Brokers, cfg.Kafka.Topic+"_payments")
	defer producer.Close()
	dispatcher := kafka.NewOutboxDispatcher(outboxRepo, producer, 5*time.Second, 100)
	go dispatcher.Start(context.Background())

	server := infrastructure.NewServer(storage)
	kafka.StartConsumer(
		context.Background(),
		cfg.Kafka.Brokers,
		cfg.Kafka.Topic,
		cfg.Kafka.ConsumerGroupID,
		db,
		server,
		inboxRepo,
		outboxRepo,
		producer,
	)

	handler := delivery.NewHandler(server)
	mux := delivery.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on port %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
