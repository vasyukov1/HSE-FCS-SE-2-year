package delivery

import (
	"encoding/json"
	"errors"
	"net/http"
	"payment-service/internal/domain"
)

// handleWithdraw godoc
// @Summary      Withdraw from account
// @Description  Decrease the balance of the specified account by the given amount
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Param        withdraw  body      domain.WithdrawRequest  true  "Account ID and amount to withdraw"
// @Success      200 {object} domain.BalanceResponse
// @Failure      400 {string} string "Invalid request payload or amount"
// @Failure      402 {string} string "Insufficient funds"
// @Failure      500 {string} string "Internal server error"
// @Router       /accounts/withdraw [post]
func (h *Handler) handleWithdraw(w http.ResponseWriter, r *http.Request) {
	var req domain.WithdrawRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, "Invalid JSON payload", http.StatusBadRequest)
		return
	}
	resp, err := h.service.Withdraw(r.Context(), req)
	if err != nil {
		if errors.Is(err, domain.ErrInvalidAmount) {
			http.Error(w, err.Error(), http.StatusBadRequest)
		} else if errors.Is(err, domain.ErrInsufficientFunds) {
			http.Error(w, err.Error(), http.StatusPaymentRequired)
		} else {
			http.Error(w, err.Error(), http.StatusInternalServerError)
		}
		return
	}
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(resp)
}
