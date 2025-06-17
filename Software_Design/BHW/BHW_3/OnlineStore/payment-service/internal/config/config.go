package config

import (
	"flag"
	"fmt"
	"os"
)

type Config struct {
	Port        string
	DatabaseURL string
}

func Load() *Config {
	cfg := &Config{}

	flag.StringVar(&cfg.Port, "PORT", os.Getenv("PORT"), "HTTP Server address")
	flag.StringVar(&cfg.DatabaseURL, "DB", os.Getenv("DATABASE_URL"), "Postgres database URL")

	flag.Parse()

	if cfg.DatabaseURL == "" {
		fmt.Fprintln(os.Stderr, "DATABASE_URL is required")
		os.Exit(1)
	}

	return cfg
}
