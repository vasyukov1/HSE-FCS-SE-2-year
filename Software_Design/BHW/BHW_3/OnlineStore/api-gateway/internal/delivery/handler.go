package delivery

import (
	"api-gateway/internal/infrastructure"
	"net/http"
	"strings"
)

type Handler struct {
	paymentService *infrastructure.PaymentClient
	orderService   *infrastructure.OrderClient
}

func NewHandler(
	paymentService *infrastructure.PaymentClient,
	orderService *infrastructure.OrderClient,
) *Handler {
	return &Handler{paymentService, orderService}
}

func (h *Handler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	switch {
	case r.Method == http.MethodPost && r.URL.Path == "/accounts/create":
		h.handleAccountCreate(w, r)
	case r.Method == http.MethodPost && r.URL.Path == "/accounts/top_up":
		h.handleTopUp(w, r)
	case r.Method == http.MethodGet && strings.HasPrefix(r.URL.Path, "/accounts/") && strings.HasSuffix(r.URL.Path, "/balance"):
		h.handleBalance(w, r)
	case r.Method == http.MethodPost && r.URL.Path == "/orders/create":
		h.handleOrderCreate(w, r)
	case r.Method == http.MethodGet && r.URL.Path == "/orders":
		h.handleOrderList(w, r)
	case r.Method == http.MethodGet && strings.HasPrefix(r.URL.Path, "/orders/") && strings.HasSuffix(r.URL.Path, "/status"):
		h.handleOrderStatus(w, r)
	default:
		http.Error(w, "Not Found", http.StatusNotFound)
	}
}
