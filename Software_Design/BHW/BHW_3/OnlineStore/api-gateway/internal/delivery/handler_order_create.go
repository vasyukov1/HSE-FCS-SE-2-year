package delivery

import (
	"api-gateway/internal/domain"
	"encoding/json"
	"github.com/google/uuid"
	"net/http"
)

// handleOrderCreate godoc
// @Summary      Create new order
// @Description  Creates an order for a user with given amount and description
// @Tags         orders
// @Accept       json
// @Produce      json
// @Param        order  body      domain.OrderRequest  true  "Order payload"
// @Success      201 {object} domain.OrderResponse
// @Failure      400 {string} string "Invalid payload or parameters"
// @Failure      502 {string} string "Create order failed"
// @Router       /orders [post]
func (h *Handler) handleOrderCreate(w http.ResponseWriter, req *http.Request) {
	var r domain.OrderRequest
	if err := json.NewDecoder(req.Body).Decode(&r); err != nil {
		http.Error(w, "Invalid JSON payload", http.StatusBadRequest)
		return
	}

	if _, err := uuid.Parse(r.UserID); err != nil {
		http.Error(w, "Invalid user_id", http.StatusBadRequest)
		return
	}
	if r.Amount <= 0 {
		http.Error(w, "Amount must be > 0", http.StatusBadRequest)
		return
	}
	if r.Description == "" {
		http.Error(w, "Description cannot be empty", http.StatusBadRequest)
		return
	}

	orderResp, err := h.orderService.Create(req.Context(), r)
	if err != nil {
		http.Error(w, "Create order failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)
	_ = json.NewEncoder(w).Encode(orderResp)
}
