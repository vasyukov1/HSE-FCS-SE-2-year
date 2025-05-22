package server

import (
	"api-gateway/internal/infrastructure"
	"net/http"
)

type Handler struct {
	analysisService *infrastructure.AnalysisClient
	fileService     *infrastructure.FileClient
}

func NewHandler(
	analysisService *infrastructure.AnalysisClient,
	fileService *infrastructure.FileClient,
) *Handler {
	return &Handler{
		analysisService,
		fileService,
	}
}

func (h *Handler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	switch {
	case r.Method == http.MethodPost && r.URL.Path == "/upload":
		h.handleUpload(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/download":
		h.handleDownload(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/analysis":
		h.handleAnalysis(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/get":
		h.handleGet(w, r)
	default:
		http.Error(w, "Not Found", http.StatusNotFound)
	}
}
