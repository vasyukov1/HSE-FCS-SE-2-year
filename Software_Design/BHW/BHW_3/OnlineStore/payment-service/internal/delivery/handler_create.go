package delivery

import (
	"encoding/json"
	"net/http"
	"payment-service/internal/domain"
)

// handleCreate godoc
// @Summary      Create new account
// @Description  Generates a new account with zero balance and returns its UUID
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Success      201 {object} map[string]string "account_id"
// @Failure      500 {string} string "Internal server error"
// @Router       /accounts [post]
func (h *Handler) handleCreate(w http.ResponseWriter, req *http.Request) {
	id, err := h.service.Create(req.Context(), domain.CreateAccountRequest{})
	if err != nil {
		http.Error(w, "Internal server error", http.StatusInternalServerError)
		return
	}

	resp := map[string]interface{}{
		"account_id": id,
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)
	json.NewEncoder(w).Encode(resp)
}
