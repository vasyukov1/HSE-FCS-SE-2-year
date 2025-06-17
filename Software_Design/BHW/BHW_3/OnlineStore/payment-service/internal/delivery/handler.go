package delivery

import (
	"net/http"
	"payment-service/internal/infrastructure"
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
	case r.Method == http.MethodPost && r.URL.Path == "/accounts":
		h.handleCreate(w, r)
	case r.Method == http.MethodPost && r.URL.Path == "/accounts/top_up":
		h.handleTopUp(w, r)
	case r.Method == http.MethodPost && r.URL.Path == "/accounts/withdraw":
		h.handleWithdraw(w, r)
	case r.Method == http.MethodGet && strings.HasPrefix(r.URL.Path, "/accounts/") && strings.HasSuffix(r.URL.Path, "/balance"):
		h.handleBalance(w, r)
	default:
		http.NotFound(w, r)
	}
}
