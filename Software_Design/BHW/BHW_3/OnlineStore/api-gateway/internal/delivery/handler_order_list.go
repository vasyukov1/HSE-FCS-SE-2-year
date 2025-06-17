package delivery

import (
	"encoding/json"
	"net/http"
)

func (h *Handler) handleOrderList(w http.ResponseWriter, req *http.Request) {
	resp, err := h.orderService.List(req.Context())
	if err != nil {
		http.Error(w, "Get orders failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(resp)
}
