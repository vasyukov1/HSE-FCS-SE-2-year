package delivery

import (
	"encoding/json"
	"net/http"
)

// handleOrderList godoc
// @Summary      List all orders
// @Description  Returns a list of all orders
// @Tags         orders
// @Accept       json
// @Produce      json
// @Success      200 {array} domain.OrderResponse
// @Failure      502 {string} string "Get orders failed"
// @Router       /orders [get]
func (h *Handler) handleOrderList(w http.ResponseWriter, req *http.Request) {
	resp, err := h.orderService.List(req.Context())
	if err != nil {
		http.Error(w, "Get orders failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(resp)
}
