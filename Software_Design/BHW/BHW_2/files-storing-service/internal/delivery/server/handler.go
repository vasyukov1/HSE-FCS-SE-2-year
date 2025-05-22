package server

import (
	"files-storing-service/internal/usecase"
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
	case r.Method == http.MethodPost && r.URL.Path == "/store":
		h.handleStore(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/get":
		h.handleGet(w, r)
	default:
		http.Error(w, "Not Found", http.StatusNotFound)
	}
}
