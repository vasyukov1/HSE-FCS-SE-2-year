package server

import (
	"github.com/google/uuid"
	"log"
	"net/http"
)

// handleGet godoc
// @Summary Get file text
// @Description Returns the content of a .txt file by ID
// @Tags storing
// @Produce text/plain
// @Param id query string true "File ID"
// @Success 200 {file} file
// @Failure 400 {string} map[string]string "Invalid ID"
// @Failure 404 {string} map[string]string "File not found"
// @Router /get [get]
func (h *Handler) handleGet(w http.ResponseWriter, r *http.Request) {
	// Parse ID
	idStr := r.URL.Query().Get("id")
	if idStr == "" {
		http.Error(w, "Missing 'id' parameter", http.StatusBadRequest)
		return
	}

	// Check ID
	id, err := uuid.Parse(idStr)
	if err != nil {
		http.Error(w, "Invalid 'id' parameter", http.StatusBadRequest)
		return
	}

	// Get file data
	data, err := h.Interactor.Get(r.Context(), id)
	if err != nil {
		http.Error(w, "Failed to read file", http.StatusBadRequest)
		return
	}

	// Forward response
	w.Header().Set("Content-Type", "text/plain")
	_, err = w.Write(data)
	if err != nil {
		log.Fatal("Error write content")
	}
}
