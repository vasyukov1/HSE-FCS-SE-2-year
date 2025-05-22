package server

import (
	"api-gateway/internal/domain"
	"net/http"
)

// handleGet godoc
// @Summary Get analysis result
// @Description Returns result (e.g., word cloud image) of the analysis for the given file ID
// @Tags analysis
// @Produce image/png
// @Param id query string true "File ID"
// @Success 200 {file} file "Analysis result"
// @Failure 400 {object} map[string]string "Missing or invalid ID"
// @Failure 502 {object} map[string]string "Failed to contact analysis service"
// @Router /get [get]
func (h *Handler) handleGet(w http.ResponseWriter, r *http.Request) {
	// Parse ID
	id := r.URL.Query().Get("id")
	if id == "" {
		http.Error(w, "Missing 'id' parameter", http.StatusBadRequest)
		return
	}

	// Get Word Cloud picture from analysis service
	resp, err := h.analysisService.GetResult(r.Context(), domain.AnalysisRequest{ID: id})
	if err != nil {
		http.Error(w, "Filed to contact analysis service: "+err.Error(), http.StatusBadGateway)
		return
	}

	// Check result
	if resp.StatusCode != http.StatusOK {
		http.Error(w, "Analysis failed: "+string(resp.Body), resp.StatusCode)
		return
	}

	// Forward response
	w.Header().Set("Content-Type", resp.ContentType)
	w.WriteHeader(resp.StatusCode)
	w.Write(resp.Body)
}
