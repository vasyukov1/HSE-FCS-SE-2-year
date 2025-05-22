package server

import (
	"github.com/google/uuid"
	"net/http"
)

// handleGet godoc
// @Summary Get analysis result
// @Description Returns generated image file for a given ID
// @Tags analysis
// @Produce octet-stream
// @Param id query string true "File ID"
// @Success 200 {file} file
// @Failure 400 {string} string "Invalid ID"
// @Failure 404 {string} string "Not found"
// @Router /get [get]
func (h *Handler) handleGet(w http.ResponseWriter, r *http.Request) {
	// Get ID
	idStr := r.URL.Query().Get("id")
	if idStr == "" {
		http.Error(w, "Missing 'id' parameter", http.StatusBadRequest)
		return
	}

	// Parse ID
	id, err := uuid.Parse(idStr)
	if err != nil {
		http.Error(w, "Invalid ID", http.StatusBadRequest)
		return
	}

	// Get analysis
	analysis, err := h.Interactor.Store.Get(r.Context(), id)
	if err != nil {
		http.Error(w, "Analysis not found", http.StatusNotFound)
		return
	}

	http.ServeFile(w, r, analysis.Location)
}
