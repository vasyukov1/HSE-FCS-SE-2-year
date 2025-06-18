package main

import (
	"context"
	"encoding/json"
	"github.com/google/uuid"
	"github.com/jackc/pgx/v5"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
	_ "order-service/internal/docs"
	"order-service/internal/domain"
	"order-service/internal/kafka"
	"order-service/internal/payment"
	"time"

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

	outboxRepo := repository.NewOutboxRepo(db)
	dispatcher := kafka.NewOutboxDispatcher(outboxRepo, producer, 5*time.Second, 100)
	go dispatcher.Start(context.Background())

	paymentClient := payment.NewPaymentClient(cfg.PaymentServiceURL)
	server := infrastructure.NewServer(storage, producer, paymentClient)

	handler := delivery.NewHandler(server)
	mux := delivery.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	handlerKafka := func(msg domain.InboxMessage) error {
		ctx := context.Background()

		switch msg.EventType {
		case "PaymentSucceeded":
			var evt domain.PaymentSucceededEvent
			if err := json.Unmarshal(msg.Payload, &evt); err != nil {
				return err
			}
			orderID, err := uuid.Parse(evt.OrderID)
			if err != nil {
				return err
			}
			return server.UpdateOrderStatus(ctx, orderID, domain.StatusFinished)

		case "PaymentFailed":
			var evt domain.PaymentFailedEvent
			if err := json.Unmarshal(msg.Payload, &evt); err != nil {
				return err
			}
			orderID, err := uuid.Parse(evt.OrderID)
			if err != nil {
				return err
			}
			return server.UpdateOrderStatus(ctx, orderID, domain.StatusFailed)

		default:
			return nil
		}
	}

	inboxRepo := repository.NewInboxRepo(db)
	kafka.StartConsumer(context.Background(),
		cfg.Kafka.Brokers,
		cfg.Kafka.Topic+"_responses",
		cfg.Kafka.ConsumerGroupID,
		inboxRepo,
		handlerKafka,
	)

	log.Printf("Listening on port %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
