package usecase

import (
	"file-analysis-service/internal/domain"
	"fmt"
	"github.com/google/uuid"
	"strings"
)

type Interactor struct {
	Store       domain.Storage
	FileFetcher domain.FileClient
	WCGenerator domain.WordCloudClient
	ImageSaver  domain.ImageSaver
}

func NewInteractor(
	store domain.Storage,
	ff domain.FileClient,
	wc domain.WordCloudClient,
	saver domain.ImageSaver,
) *Interactor {
	return &Interactor{store, ff, wc, saver}
}

func (i *Interactor) Analyze(ctx domain.Context, idStr string) (domain.Analysis, error) {
	// Parse ID
	id, err := uuid.Parse(idStr)
	if err != nil {
		return domain.Analysis{}, err
	}

	// Check analysis in cash
	if analysis, err := i.Store.Get(ctx, id); err == nil {
		return analysis, nil
	}

	// Get data from storage
	raw, err := i.FileFetcher.FetchFile(ctx, id)
	if err != nil {
		return domain.Analysis{}, fmt.Errorf("failed to fetch file: %w", err)
	}

	// Count stats
	wordCount, paragraphCount, characterCount := countStats(string(raw))

	// Generate Word Cloud
	imgData, err := i.WCGenerator.Generate(ctx, raw)
	if err != nil {
		return domain.Analysis{}, err
	}

	// Save picture
	path, err := i.ImageSaver.Save(id.String(), imgData)
	if err != nil {
		return domain.Analysis{}, err
	}

	// Create analysis object
	analysis := domain.Analysis{
		ID:             id,
		Location:       path,
		WordCount:      wordCount,
		ParagraphCount: paragraphCount,
		CharacterCount: characterCount,
	}

	// Save data in DB
	if err := i.Store.Save(ctx, analysis); err != nil {
		return domain.Analysis{}, err
	}

	return analysis, nil
}

func countStats(text string) (wordCount, paragraphCount, characterCount int) {
	wordCount = len(strings.Fields(text))

	paragraphCount = len(strings.Split(strings.ReplaceAll(text, "\r\n", "\n"), "\n\n"))

	characterCount = len(text)

	return wordCount, paragraphCount, characterCount
}
