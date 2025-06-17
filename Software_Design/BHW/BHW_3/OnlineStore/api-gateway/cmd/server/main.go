package main

import (
	"api-gateway/internal/config"
	"api-gateway/internal/delivery"
	"api-gateway/internal/infrastructure"
	httpSwagger "github.com/swaggo/http-swagger"
	"log"
	"net/http"
)

// @title API Gateway
// @version 1.0
// @description Gateway is for distributing requests between microservices
// @BasePath /
func main() {
	cfg := config.Load()

	paymentService := infrastructure.NewPaymentClient(cfg.PaymentService)
	orderService := infrastructure.NewOrderClient(cfg.OrderService)

	handler := delivery.NewHandler(paymentService, orderService)
	mux := delivery.NewRouter(handler)

	mux.Handle("/swagger/", httpSwagger.WrapHandler)

	log.Printf("Listening on port %s", cfg.Port)
	log.Fatal(http.ListenAndServe(cfg.Port, mux))
}
