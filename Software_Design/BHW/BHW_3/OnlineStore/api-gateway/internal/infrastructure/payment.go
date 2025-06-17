package infrastructure

import (
	"api-gateway/internal/domain"
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)

type PaymentClient struct {
	baseURL    string
	httpClient *http.Client
}

func NewPaymentClient(baseURL string) *PaymentClient {
	return &PaymentClient{
		baseURL:    baseURL,
		httpClient: &http.Client{},
	}
}

func (c *PaymentClient) CreateAccount(ctx context.Context) (*domain.PaymentResponse, error) {
	url := fmt.Sprintf("%s/accounts", c.baseURL)

	httpReq, err := http.NewRequestWithContext(ctx, http.MethodPost, url, nil)
	if err != nil {
		return nil, err
	}
	httpReq.Header.Set("Accept", "application/json")

	resp, err := c.httpClient.Do(httpReq)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}

	if resp.StatusCode != http.StatusCreated {
		return nil, fmt.Errorf("top up failed: status %d, body %s", resp.StatusCode, string(body))
	}

	return &domain.PaymentResponse{
		ContentType: resp.Header.Get("Content-Type"),
		Body:        body,
		StatusCode:  resp.StatusCode,
	}, nil
}

func (c *PaymentClient) Balance(ctx context.Context, req domain.BalanceRequest) (*domain.BalanceResponse, error) {
	url := fmt.Sprintf("%s/accounts/%s/balance", c.baseURL, req.AccountID)

	httpReq, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return nil, err
	}
	httpReq.Header.Set("Accept", "application/json")

	resp, err := c.httpClient.Do(httpReq)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("get balance failed: status %d, body %s", resp.StatusCode, string(body))
	}

	var out domain.BalanceResponse
	if err := json.Unmarshal(body, &out); err != nil {
		return nil, fmt.Errorf("invalid JSON from payment-service: %w", err)
	}

	return &out, nil
}

func (c *PaymentClient) TopUp(ctx context.Context, req domain.TopUpRequest) (*domain.PaymentResponse, error) {
	url := fmt.Sprintf("%s/accounts/top_up", c.baseURL)

	payload := struct {
		AccountID string  `json:"account_id"`
		Amount    float64 `json:"amount"`
	}{
		AccountID: req.AccountID,
		Amount:    req.Amount,
	}
	data, err := json.Marshal(payload)
	if err != nil {
		return nil, fmt.Errorf("failed to marshal payload: %w", err)
	}

	httpReq, err := http.NewRequestWithContext(ctx, http.MethodPost, url, bytes.NewBuffer(data))
	if err != nil {
		return nil, err
	}
	httpReq.Header.Set("Content-Type", "application/json")
	httpReq.Header.Set("Accept", "application/json")

	resp, err := c.httpClient.Do(httpReq)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("top up failed: status %d, body %s", resp.StatusCode, string(body))
	}

	return &domain.PaymentResponse{
		ContentType: resp.Header.Get("Content-Type"),
		Body:        body,
		StatusCode:  resp.StatusCode,
	}, nil
}
