package delivery

import (
	"encoding/json"
	"errors"
	"github.com/google/uuid"
	"log"
	"net/http"
	"payment-service/internal/domain"
	"payment-service/internal/repository"
)

// handleTopUp godoc
// @Summary      Top up account balance
// @Description  Increase the balance of the specified account by the given amount
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Param        top_up  body      domain.TopUpRequest  true  "Account ID and amount"
// @Success      200 {object} domain.BalanceResponse
// @Failure      400 {string} string "Invalid request payload or amount"
// @Failure      404 {string} string "Account not found"
// @Failure      500 {string} string "Internal server error"
// @Router       /accounts/top_up [post]
func (h *Handler) handleTopUp(w http.ResponseWriter, req *http.Request) {
	var r domain.TopUpRequest
	if err := json.NewDecoder(req.Body).Decode(&r); err != nil {
		http.Error(w, "Invalid request payload", http.StatusBadRequest)
		return
	}

	if r.AccountID == uuid.Nil {
		http.Error(w, "Invalid account_id", http.StatusBadRequest)
		return
	}

	res, err := h.service.TopUp(req.Context(), r)
	if err != nil {
		switch {
		case errors.Is(err, domain.ErrInvalidAmount):
			http.Error(w, err.Error(), http.StatusBadRequest)
		case errors.Is(err, repository.ErrAccountNotFound):
			http.Error(w, "Account not found", http.StatusNotFound)
		default:
			http.Error(w, "Internal server error", http.StatusInternalServerError)
		}
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	if err := json.NewEncoder(w).Encode(res); err != nil {
		log.Printf("handleTopUp: failed to encode response: %v", err)
	}
}
