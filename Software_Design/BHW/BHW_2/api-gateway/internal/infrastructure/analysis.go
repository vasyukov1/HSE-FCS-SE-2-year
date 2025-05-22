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

type AnalysisClient struct {
	baseURL    string
	httpClient *http.Client
}

func NewAnalysisClient(baseURL string) *AnalysisClient {
	return &AnalysisClient{
		baseURL:    baseURL,
		httpClient: &http.Client{},
	}
}

func (c *AnalysisClient) Analyze(ctx context.Context, req domain.AnalysisRequest) (*domain.AnalysisResponse, error) {
	// Create request
	requestBody := struct{ ID string }{ID: req.ID}
	jsonBody, err := json.Marshal(requestBody)
	if err != nil {
		return nil, err
	}

	// Make request to Analysis Service
	httpReq, err := http.NewRequestWithContext(
		ctx,
		"POST",
		c.baseURL+"/analysis",
		bytes.NewBuffer(jsonBody),
	)
	if err != nil {
		return nil, err
	}
	httpReq.Header.Set("Content-Type", "application/json")

	// Get response
	resp, err := c.httpClient.Do(httpReq)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	// Read response
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	// Forward response
	return &domain.AnalysisResponse{
		ContentType: resp.Header.Get("Content-Type"),
		Body:        body,
		StatusCode:  resp.StatusCode,
	}, nil
}

func (c *AnalysisClient) GetResult(ctx context.Context, req domain.AnalysisRequest) (*domain.AnalysisResponse, error) {
	// Make request to Analysis Service
	httpReq, err := http.NewRequestWithContext(
		ctx,
		"GET",
		c.baseURL+"/get?id="+req.ID,
		nil,
	)
	if err != nil {
		return nil, err
	}

	// Get response
	resp, err := c.httpClient.Do(httpReq)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	// Read response
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %w", err)
	}

	// Forward response
	return &domain.AnalysisResponse{
		ContentType: resp.Header.Get("Content-Type"),
		Body:        body,
		StatusCode:  resp.StatusCode,
	}, nil
}
