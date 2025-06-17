package delivery

import (
	"api-gateway/internal/domain"
	"encoding/json"
	"net/http"
	"strings"
)

// handleBalance godoc
// @Summary      Get account balance
// @Description  Return current balance for the given account ID
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Param        account_id  query   string  true  "Account UUID"
// @Success      200 {object} domain.BalanceResponse
// @Failure      400 {string} string "Missing 'account_id' parameter"
// @Failure      502 {string} string "Get balance failed"
// @Router       /balance [get]
func (h *Handler) handleBalance(w http.ResponseWriter, req *http.Request) {
	parts := strings.Split(req.URL.Path, "/")
	if len(parts) < 4 || parts[1] != "accounts" || parts[3] != "balance" {
		http.Error(w, "Invalid path", http.StatusBadRequest)
		return
	}
	accountID := parts[2]
	if accountID == "" {
		http.Error(w, "Missing account_id in path", http.StatusBadRequest)
		return
	}

	resp, err := h.paymentService.Balance(req.Context(), domain.BalanceRequest{AccountID: accountID})
	if err != nil {
		http.Error(w, "Get balance failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(resp)
}
