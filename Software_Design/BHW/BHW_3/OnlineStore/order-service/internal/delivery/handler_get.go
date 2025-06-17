package delivery

import (
	"encoding/json"
	"log"
	"net/http"
)

// handleGet godoc
// @Summary      List all orders
// @Description  Returns a JSON array of all orders in the system
// @Tags         orders
// @Accept       json
// @Produce      json
// @Success      200 {array}  domain.Order
// @Failure      500 {string} string "Internal server error"
// @Router       /orders [get]
func (h *Handler) handleGet(w http.ResponseWriter, req *http.Request) {
	orders, err := h.service.List(req.Context())
	if err != nil {
		http.Error(w, "Failed to list orders: "+err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	if err := json.NewEncoder(w).Encode(orders); err != nil {
		log.Printf("handleGet: failed to encode response: %v", err)
	}
}
