package config

import (
	"flag"
	"fmt"
	"os"
)

type Config struct {
	Port            string
	FileService     string
	AnalysisService string
}

func Load() *Config {
	cfg := &Config{}

	flag.StringVar(&cfg.Port, "addr", os.Getenv("PORT"),
		"HTTP server address")
	flag.StringVar(&cfg.FileService, "FILE_STORAGE_URL", os.Getenv("FILE_STORAGE_URL"),
		"File storage service URL (env FILE_STORAGE_URL)")
	flag.StringVar(&cfg.AnalysisService, "ANALYSIS_SERVICE_URL", os.Getenv("ANALYSIS_SERVICE_URL"),
		"File analysis service URL (env ANALYSIS_SERVICE_URL)")

	flag.Parse()

	if cfg.FileService == "" {
		fmt.Fprintln(os.Stderr, "FILE_STORAGE_URL is required")
		os.Exit(1)
	}
	if cfg.AnalysisService == "" {
		fmt.Fprintln(os.Stderr, "ANALYSIS_SERVICE_URL is required")
		os.Exit(1)
	}
	return cfg
}
