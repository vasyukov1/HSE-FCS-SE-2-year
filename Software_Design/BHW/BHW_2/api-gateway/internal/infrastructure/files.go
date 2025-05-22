package infrastructure

import (
	"api-gateway/internal/domain"
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"mime/multipart"
	"net/http"
)

type FileClient struct {
	baseURL    string
	httpClient *http.Client
}

func NewFileClient(baseURL string) *FileClient {
	return &FileClient{
		baseURL:    baseURL,
		httpClient: &http.Client{},
	}
}

func (c *FileClient) Store(ctx context.Context, file *domain.File) (string, error) {
	body := &bytes.Buffer{}
	writer := multipart.NewWriter(body)

	part, err := writer.CreateFormFile("file", file.Name)
	if err != nil {
		return "", fmt.Errorf("create form file: %w", err)
	}

	if _, err := io.Copy(part, file.Content); err != nil {
		return "", fmt.Errorf("copy content: %w", err)
	}
	writer.Close()

	req, err := http.NewRequestWithContext(
		ctx,
		"POST",
		c.baseURL+"/store",
		body,
	)
	if err != nil {
		return "", fmt.Errorf("create request: %w", err)
	}
	req.Header.Set("Content-Type", writer.FormDataContentType())

	resp, err := c.httpClient.Do(req)
	if err != nil {
		return "", fmt.Errorf("execute request: %w", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return "", fmt.Errorf("unexpected status code: %d", resp.StatusCode)
	}

	var result struct{ ID string }
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return "", fmt.Errorf("decode response: %w", err)
	}

	return result.ID, nil
}

func (c *FileClient) Get(ctx context.Context, id string) (*domain.File, error) {
	req, err := http.NewRequestWithContext(
		ctx,
		"GET",
		fmt.Sprintf("%s/get?id=%s", c.baseURL, id),
		nil,
	)
	if err != nil {
		return nil, fmt.Errorf("create request: %w", err)
	}

	resp, err := c.httpClient.Do(req)
	if err != nil {
		return nil, fmt.Errorf("execute request: %w", err)
	}

	if resp.StatusCode != http.StatusOK {
		defer resp.Body.Close()
		return nil, fmt.Errorf("unexpected status code: %d", resp.StatusCode)
	}

	return &domain.File{
		Content: resp.Body,
	}, nil
}
