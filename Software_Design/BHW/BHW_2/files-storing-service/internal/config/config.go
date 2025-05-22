package config

import (
	"flag"
	"fmt"
	"os"
)

type Config struct {
	Port        string
	DatabaseURL string
	BaseDir     string
}

func Load() *Config {
	c := &Config{}

	flag.StringVar(&c.Port, "addr", os.Getenv("PORT"), "HTTP address")
	flag.StringVar(&c.DatabaseURL, "db", os.Getenv("DATABASE_URL"), "Postgres DSN")
	flag.StringVar(&c.BaseDir, "dir", os.Getenv("FILES_DIR"), "Directory to store files")
	flag.Parse()

	if c.DatabaseURL == "" {
		fmt.Fprintln(os.Stderr, "DATABASE_URL is required")
		os.Exit(1)
	}

	return c
}
