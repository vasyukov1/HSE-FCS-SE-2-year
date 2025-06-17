package delivery

import (
	"net/http"
)

// handleAccountCreate godoc
// @Summary      Proxy account creation
// @Description  Proxies account creation request to payment-service
// @Tags         accounts
// @Accept       json
// @Produce      json
// @Success      201 {object} map[string]string "account_id"
// @Failure      502 {string} string "Create account failed"
// @Router       /accounts/create [post]
func (h *Handler) handleAccountCreate(w http.ResponseWriter, req *http.Request) {
	resp, err := h.paymentService.CreateAccount(req.Context())
	if err != nil {
		http.Error(w, "Create account failed", http.StatusBadGateway)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(resp.StatusCode)
	w.Write(resp.Body)
}
