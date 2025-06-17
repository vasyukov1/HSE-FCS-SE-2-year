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

type OrderClient struct {
	baseURL    string
	httpClient *http.Client
}

func NewOrderClient(baseURL string) *OrderClient {
	return &OrderClient{
		baseURL:    baseURL,
		httpClient: &http.Client{},
	}
}

func (c *OrderClient) Create(ctx context.Context, req domain.OrderRequest) (*domain.OrderResponse, error) {
	url := fmt.Sprintf("%s/orders", c.baseURL)

	data, err := json.Marshal(req)
	if err != nil {
		return nil, fmt.Errorf("failed to marshal order request: %w", err)
	}

	httpReq, err := http.NewRequestWithContext(ctx, http.MethodPost, url, bytes.NewReader(data))
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
	if resp.StatusCode != http.StatusCreated {
		return nil, fmt.Errorf("create order failed: status %d, body %s", resp.StatusCode, string(body))
	}

	var out domain.OrderResponse
	if err := json.Unmarshal(body, &out); err != nil {
		return nil, fmt.Errorf("invalid JSON from order-service: %w", err)
	}
	return &out, nil
}

func (c *OrderClient) List(ctx context.Context) ([]domain.OrderResponse, error) {
	url := fmt.Sprintf("%s/orders", c.baseURL)

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return nil, err
	}
	req.Header.Set("Accept", "application/json")

	resp, err := c.httpClient.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("get orders failed: status %d, body %s", resp.StatusCode, string(body))
	}

	var out []domain.OrderResponse
	if err := json.Unmarshal(body, &out); err != nil {
		return nil, fmt.Errorf("invalid JSON from payment-service: %w", err)
	}

	return out, nil
}

func (c *OrderClient) Status(ctx context.Context, orderID string) (*domain.StatusResponse, error) {
	url := fmt.Sprintf("%s/orders/%s/status", c.baseURL, orderID)

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return nil, err
	}
	req.Header.Set("Accept", "application/json")

	resp, err := c.httpClient.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("get order status failed: status %d, body %s", resp.StatusCode, string(body))
	}

	var out domain.StatusResponse
	if err := json.Unmarshal(body, &out); err != nil {
		return nil, fmt.Errorf("invalid JSON from order-service: %w", err)
	}
	return &out, nil
}
