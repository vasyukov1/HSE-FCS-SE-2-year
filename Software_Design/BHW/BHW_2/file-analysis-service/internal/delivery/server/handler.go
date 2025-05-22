package server

import (
	"file-analysis-service/internal/usecase"
	"net/http"
)

type Handler struct {
	Interactor *usecase.Interactor
}

func NewHandler(i *usecase.Interactor) *Handler {
	return &Handler{Interactor: i}
}

func (h *Handler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	switch {
	case r.Method == http.MethodPost && r.URL.Path == "/analysis":
		h.handleAnalyze(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/get":
		h.handleGet(w, r)
	default:
		http.Error(w, "Not Found", http.StatusNotFound)
	}
}
