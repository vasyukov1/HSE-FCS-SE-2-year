package wordcloud

import (
	"bytes"
	"encoding/json"
	"file-analysis-service/internal/domain"
	"io"
	"net/http"
)

type Payload struct {
	Format    string `json:"format"`
	Width     int    `json:"width"`
	Height    int    `json:"height"`
	Text      string `json:"text"`
	FontScale int    `json:"fontScale"`
}

type Client struct{}

func NewClient() domain.WordCloudClient {
	return &Client{}
}

func (c *Client) Generate(ctx domain.Context, text []byte) ([]byte, error) {
	// Generate request for Word Cloud API
	p := Payload{
		Format:    "png",
		Width:     500,
		Height:    500,
		Text:      string(text),
		FontScale: 15,
	}

	// Create JSON
	jsonBody, err := json.Marshal(p)
	if err != nil {
		return nil, err
	}

	// Get response
	resp, err := http.Post(
		"https://quickchart.io/wordcloud",
		"application/json",
		bytes.NewBuffer(jsonBody),
	)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	return io.ReadAll(resp.Body)
}
