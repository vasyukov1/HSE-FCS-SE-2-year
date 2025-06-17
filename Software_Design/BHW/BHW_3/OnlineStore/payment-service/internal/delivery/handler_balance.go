package delivery

import (
	"encoding/json"
	"github.com/google/uuid"
	"log"
	"net/http"
	"payment-service/internal/domain"
	"strings"
)

// handleBalance godoc
// @Summary      Get account balance
// @Description  Returns the current balance for the given account
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Param        account_id   path      string  true  "Account UUID"
// @Success      200 {object}  domain.BalanceResponse
// @Failure      400 {string}  string  "Invalid account_id"
// @Failure      500 {string}  string  "Get balance failed"
// @Router       /accounts/{account_id}/balance [get]
func (h *Handler) handleBalance(w http.ResponseWriter, req *http.Request) {
	parts := strings.Split(req.URL.Path, "/")
	if len(parts) != 4 {
		http.Error(w, "Not found", http.StatusNotFound)
		return
	}

	idStr := parts[2]
	id, err := uuid.Parse(idStr)
	if err != nil {
		http.Error(w, "Invalid account_id", http.StatusBadRequest)
		return
	}

	data, err := h.service.Balance(req.Context(), domain.BalanceRequest{AccountID: id})
	if err != nil {
		http.Error(w, "Get balance failed: "+err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	if err := json.NewEncoder(w).Encode(data); err != nil {
		log.Printf("failed to encode response: %v", err)
	}
}
