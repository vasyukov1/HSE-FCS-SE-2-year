package delivery

import (
	"api-gateway/internal/domain"
	"encoding/json"
	"net/http"
)

// handleTopUp godoc
// @Summary      Top up account balance
// @Description  Increases the balance of the specified account by the given amount
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Param        top_up_request  body      domain.TopUpRequest  true  "Account ID and amount to top up"
// @Success      200 {object} map[string]interface{}  "account_id, new_balance"
// @Failure      400 {string} string "Invalid request payload"
// @Failure      502 {string} string "Top up failed"
// @Router       /accounts/top_up [post]
func (h *Handler) handleTopUp(w http.ResponseWriter, req *http.Request) {
	var r domain.TopUpRequest
	if err := json.NewDecoder(req.Body).Decode(&r); err != nil {
		http.Error(w, "Invalid request payload", http.StatusBadRequest)
		return
	}

	if r.AccountID == "" {
		http.Error(w, "Missing account_id", http.StatusBadRequest)
		return
	}
	if r.Amount <= 0 {
		http.Error(w, "Amount must be > 0", http.StatusBadRequest)
		return
	}

	resp, err := h.paymentService.TopUp(req.Context(), r)
	if err != nil {
		http.Error(w, "Top up failed: "+err.Error(), http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", resp.ContentType)
	w.WriteHeader(resp.StatusCode)
	w.Write(resp.Body)
}
