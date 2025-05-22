package server

import (
	"encoding/json"
	"file-analysis-service/internal/domain"
	"net/http"
)

// handleAnalyze godoc
// @Summary Analyze file
// @Description Generates word cloud image from text file by ID
// @Tags analysis
// @Accept json
// @Produce json
// @Param request body domain.AnalyzeRequest true "File ID"
// @Success 200 {object} map[string]interface{}
// @Failure 400 {string} string "Invalid JSON"
// @Failure 500 {string} string "Internal error"
// @Router /analysis [post]
func (h *Handler) handleAnalyze(w http.ResponseWriter, r *http.Request) {
	// Parse ID
	var req domain.AnalyzeRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, "Invalid JSON", http.StatusBadRequest)
		return
	}

	// Analysis of data
	analysis, err := h.Interactor.Analyze(r.Context(), req.ID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	// Forward response
	resp := map[string]interface{}{
		"image_url":       analysis.Location,
		"word_count":      analysis.WordCount,
		"paragraph_count": analysis.ParagraphCount,
		"character_count": analysis.CharacterCount,
	}

	json.NewEncoder(w).Encode(resp)
}
