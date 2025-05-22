package config

import (
	"flag"
	"fmt"
	"os"
)

type Config struct {
	Port            string
	DatabaseURL     string
	FileStorageURL  string
	WordCloudAPIURL string
	ImagesDir       string
}

func Load() *Config {
	cfg := &Config{}

	flag.StringVar(&cfg.Port, "addr", os.Getenv("PORT"),
		"HTTP server address")
	flag.StringVar(&cfg.DatabaseURL, "db", os.Getenv("DATABASE_URL"),
		"Postgres DSN")
	flag.StringVar(&cfg.FileStorageURL, "file-url", os.Getenv("FILE_STORAGE_URL"),
		"File storage service URL")
	flag.StringVar(&cfg.WordCloudAPIURL, "wc-url", os.Getenv("WORD_CLOUD_API_URL"),
		"Word Cloud API URL")
	flag.StringVar(&cfg.ImagesDir, "images-dir", os.Getenv("IMAGES_DIR"),
		"Directory to save generated images")

	flag.Parse()

	if cfg.DatabaseURL == "" {
		fmt.Fprintln(os.Stderr, "DATABASE_URL is required")
		os.Exit(1)
	}
	if cfg.FileStorageURL == "" {
		fmt.Fprintln(os.Stderr, "FILE_STORAGE_URL is required")
		os.Exit(1)
	}
	return cfg
}
