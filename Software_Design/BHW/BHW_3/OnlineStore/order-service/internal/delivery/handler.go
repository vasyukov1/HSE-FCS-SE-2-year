package delivery

import (
	"net/http"
	"order-service/internal/infrastructure"
	"strings"
)

type Handler struct {
	service *infrastructure.Server
}

func NewHandler(service *infrastructure.Server) *Handler {
	return &Handler{service}
}

func (h *Handler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	switch {
	case r.Method == http.MethodPost && r.URL.Path == "/orders":
		h.handleCreate(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/orders":
		h.handleGet(w, r)
	case r.Method == http.MethodGet && strings.HasPrefix(r.URL.Path, "/orders/") && strings.HasSuffix(r.URL.Path, "/status"):
		h.handleStatus(w, r)
	default:
		http.NotFound(w, r)
	}
}
