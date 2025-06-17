package delivery

import (
	"encoding/json"
	"log"
	"net/http"
	"order-service/internal/domain"
	"strings"
)

// handleCreate godoc
// @Summary      Create new order
// @Description  Creates an order (validates user_id, amount>0, description) and returns it
// @Tags         orders
// @Accept       json
// @Produce      json
// @Param        order  body      domain.CreateOrderRequest  true  "Order payload"
// @Success      201 {object} domain.Order
// @Failure      400 {string} string "Invalid request"
// @Failure      402 {string} string "Insufficient funds"
// @Failure      500 {string} string "Internal server error"
// @Router       /orders [post]
func (h *Handler) handleCreate(w http.ResponseWriter, r *http.Request) {
	var req domain.CreateOrderRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, "Invalid JSON payload", http.StatusBadRequest)
		return
	}

	order, err := h.service.Create(r.Context(), req)
	if err != nil {
		if strings.Contains(err.Error(), "insufficient funds") {
			http.Error(w, "Insufficient funds", http.StatusPaymentRequired)
			return
		}
		switch err {
		case domain.ErrInvalidAmount, domain.ErrInvalidUserID:
			http.Error(w, err.Error(), http.StatusBadRequest)
		default:
			http.Error(w, "Failed to create order: "+err.Error(), http.StatusInternalServerError)
		}
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)
	if err := json.NewEncoder(w).Encode(order); err != nil {
		log.Printf("handleCreate: encode error: %v", err)
	}
}
