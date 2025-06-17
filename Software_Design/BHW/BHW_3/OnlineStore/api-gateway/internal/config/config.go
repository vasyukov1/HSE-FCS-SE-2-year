package config

import (
	"flag"
	"log"
	"os"
)

type Config struct {
	Port           string
	OrderService   string
	PaymentService string
}

func Load() *Config {
	config := &Config{}

	flag.StringVar(&config.Port, "PORT", os.Getenv("PORT"), "HTTP server address")
	flag.StringVar(&config.OrderService, "ORDER_SERVICE_URL", os.Getenv("ORDER_SERVICE_URL"), "ORDER SERVICE URL")
	flag.StringVar(&config.PaymentService, "PAYMENT_SERVICE_URL", os.Getenv("PAYMENT_SERVICE_URL"), "PAYMENT SERVICE URL")

	flag.Parse()

	if config.OrderService == "" {
		log.Print("ORDER_SERVICE_URL is required")
		os.Exit(1)
	}
	if config.PaymentService == "" {
		log.Print("PAYMENT_SERVICE_URL is required")
		os.Exit(1)
	}

	return config
}
