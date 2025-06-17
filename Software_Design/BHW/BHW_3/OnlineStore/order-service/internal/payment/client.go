package payment

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"github.com/google/uuid"
	"net/http"
	"time"
)

type PaymentClient struct {
	BaseURL string
	Client  *http.Client
}

type WithdrawRequest struct {
	AccountID uuid.UUID `json:"account_id"`
	Amount    float64   `json:"amount"`
}

type WithdrawResponse struct {
	AccountID string  `json:"account_id"`
	Balance   float64 `json:"balance"`
}

func NewPaymentClient(baseURL string) *PaymentClient {
	return &PaymentClient{
		BaseURL: baseURL,
		Client:  &http.Client{Timeout: 5 * time.Second},
	}
}

func (pc *PaymentClient) Withdraw(ctx context.Context, accountID uuid.UUID, amount float64) (*WithdrawResponse, error) {
	reqBody, err := json.Marshal(WithdrawRequest{
		AccountID: accountID,
		Amount:    amount,
	})
	if err != nil {
		return nil, err
	}

	req, err := http.NewRequestWithContext(ctx, http.MethodPost, pc.BaseURL+"/accounts/withdraw", bytes.NewReader(reqBody))
	if err != nil {
		return nil, err
	}
	req.Header.Set("Content-Type", "application/json")

	resp, err := pc.Client.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode == http.StatusPaymentRequired {
		return nil, fmt.Errorf("insufficient funds")
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("payment service error: %s", resp.Status)
	}

	var wr WithdrawResponse
	if err := json.NewDecoder(resp.Body).Decode(&wr); err != nil {
		return nil, err
	}
	return &wr, nil
}
