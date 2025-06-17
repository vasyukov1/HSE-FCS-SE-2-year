package delivery

import (
	"encoding/json"
	"net/http"
	"strings"
)

func (h *Handler) handleOrderStatus(w http.ResponseWriter, req *http.Request) {
	parts := strings.Split(req.URL.Path, "/")
	if len(parts) != 4 || parts[1] != "orders" || parts[3] != "status" {
		http.NotFound(w, req)
		return
	}
	orderID := parts[2]

	statusResp, err := h.orderService.Status(req.Context(), orderID)
	if err != nil {
		http.Error(w, "Get order status failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(statusResp)
}
