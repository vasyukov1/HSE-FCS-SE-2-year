package delivery

import (
	"encoding/json"
	"github.com/google/uuid"
	"log"
	"net/http"
	"strings"
)

// handleStatus godoc
// @Summary      Get order status
// @Description  Returns the status field for the given order
// @Tags         orders
// @Accept       json
// @Produce      json
// @Param        order_id  path      string  true  "Order UUID"
// @Success      200 {object} map[string]string  "id and status"
// @Failure      400 {string} string "Invalid order_id"
// @Failure      404 {string} string "Order not found"
// @Failure      500 {string} string "Internal server error"
// @Router       /orders/{order_id}/status [get]
func (h *Handler) handleStatus(w http.ResponseWriter, req *http.Request) {
	parts := strings.Split(req.URL.Path, "/")
	if len(parts) != 4 || parts[1] != "orders" || parts[3] != "status" {
		http.NotFound(w, req)
		return
	}
	idStr := parts[2]
	id, err := uuid.Parse(idStr)
	if err != nil {
		http.Error(w, "Invalid order_id", http.StatusBadRequest)
		return
	}

	order, err := h.service.GetByID(req.Context(), id)
	if err != nil {
		http.Error(w, "Internal server error", http.StatusInternalServerError)
		return
	}
	if order == nil {
		http.Error(w, "Order not found", http.StatusNotFound)
		return
	}

	resp := map[string]string{
		"id":     order.ID.String(),
		"status": string(order.Status),
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	if err := json.NewEncoder(w).Encode(resp); err != nil {
		log.Printf("handleStatus: encode error: %v", err)
	}
}
