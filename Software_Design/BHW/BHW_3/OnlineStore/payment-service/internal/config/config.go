package config

import (
	"flag"
	"fmt"
	"os"
	"strings"
)

type Config struct {
	Port        string
	DatabaseURL string
	Kafka       struct {
		Brokers         []string
		Topic           string
		ConsumerGroupID string
	}
}

func Load() *Config {
	cfg := &Config{}

	var kafkaBrokers string

	flag.StringVar(&cfg.Port, "PORT", os.Getenv("PORT"), "HTTP Server address")
	flag.StringVar(&cfg.DatabaseURL, "DB", os.Getenv("DATABASE_URL"), "Postgres database URL")
	flag.StringVar(&cfg.Kafka.Topic, "KAFKA_TOPIC", os.Getenv("KAFKA_TOPIC"), "Kafka topic name")
	flag.StringVar(&kafkaBrokers, "KAFKA_BROKERS", os.Getenv("KAFKA_BROKERS"), "Kafka brokers, comma-separated")
	flag.StringVar(&cfg.Kafka.ConsumerGroupID, "KAFKA_CONSUMER_GROUP", os.Getenv("KAFKA_CONSUMER_GROUP"), "Kafka consumer group ID")

	flag.Parse()

	if kafkaBrokers != "" {
		cfg.Kafka.Brokers = strings.Split(kafkaBrokers, ",")
	}

	if cfg.DatabaseURL == "" {
		fmt.Fprintln(os.Stderr, "DATABASE_URL is required")
		os.Exit(1)
	}

	return cfg
}
