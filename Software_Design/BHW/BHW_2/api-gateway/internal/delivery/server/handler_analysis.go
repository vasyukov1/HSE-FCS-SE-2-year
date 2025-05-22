package server

import (
	"api-gateway/internal/domain"
	"encoding/json"
	"net/http"
)

// handleAnalysis godoc
// @Summary Analyze file
// @Description Starts analysis of the file with the given ID
// @Tags analysis
// @Produce json
// @Param id query string true "File ID"
// @Success 200 {object} domain.AnalyzedData "Analysis result"
// @Failure 400 {object} map[string]string "Missing or invalid ID"
// @Failure 500 {object} map[string]string "Internal analysis error"
// @Router /analysis [get]
func (h *Handler) handleAnalysis(w http.ResponseWriter, r *http.Request) {
	// Parse ID
	id := r.URL.Query().Get("id")
	if id == "" {
		http.Error(w, "Missing 'id' parameter", http.StatusBadRequest)
		return
	}

	// Get Analysis
	resp, err := h.analysisService.Analyze(r.Context(), domain.AnalysisRequest{ID: id})
	if err != nil {
		http.Error(w, "Internal server error", http.StatusInternalServerError)
		return
	}

	// Forward response
	var result domain.AnalyzedData
	if err := json.Unmarshal(resp.Body, &result); err != nil {
		http.Error(w, "Failed to parse analysis result", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(result)
}
