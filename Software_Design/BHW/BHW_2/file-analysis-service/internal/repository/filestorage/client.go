package filestorage

import (
	"file-analysis-service/internal/domain"
	"fmt"
	"github.com/google/uuid"
	"io"
	"net/http"
)

type Client struct {
	BaseURL string
}

func NewClient(baseURL string) domain.FileClient {
	return &Client{BaseURL: baseURL}
}

func (c *Client) FetchFile(ctx domain.Context, id uuid.UUID) ([]byte, error) {
	// Get data from storage
	resp, err := http.Get(fmt.Sprintf("%s/get?id=%s", c.BaseURL, id.String()))
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	// Check existing of data
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("file fetch failed: status %d", resp.StatusCode)
	}

	// Read data
	data, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}
	return data, nil
}
