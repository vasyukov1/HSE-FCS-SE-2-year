package domain

import "context"

type AnalysisRequest struct {
	ID string
}

type AnalysisResponse struct {
	ContentType string
	Body        []byte
	StatusCode  int
}

type AnalyzedData struct {
	ImageURL       string `json:"image_url"`
	WordCount      int    `json:"word_count"`
	ParagraphCount int    `json:"paragraph_count"`
	CharacterCount int    `json:"character_count"`
}

type AnalysisService interface {
	Analyze(ctx context.Context, req AnalysisRequest) (*AnalysisResponse, error)
	GetResult(ctx context.Context, req AnalysisRequest) (*AnalysisResponse, error)
}
