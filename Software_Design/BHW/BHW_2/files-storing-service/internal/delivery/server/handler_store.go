package server

import (
	"encoding/json"
	"io"
	"net/http"
	"path/filepath"
)

// handleStore godoc
// @Summary Store file
// @Description Upload and store a .txt file on the server
// @Tags storing
// @Accept multipart/form-data
// @Produce json
// @Param file formData file true "Text file to upload"
// @Success 200 {object} map[string]string
// @Failure 400 {object} map[string]string "Invalid input"
// @Failure 500 {object} map[string]string "Internal error"
// @Router /store [post]
func (h *Handler) handleStore(w http.ResponseWriter, r *http.Request) {
	// Get file
	file, header, err := r.FormFile("file")
	if err != nil {
		http.Error(w, "File not provided", http.StatusBadRequest)
		return
	}
	defer file.Close()

	// Check file format
	if filepath.Ext(header.Filename) != ".txt" {
		http.Error(w, "Only .txt files are allowed", http.StatusBadRequest)
		return
	}

	// Read file
	data, err := io.ReadAll(file)
	if err != nil {
		http.Error(w, "Failed to read file", http.StatusInternalServerError)
		return
	}

	// Save file
	id, err := h.Interactor.Store(r.Context(), header.Filename, data)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	// Forward response
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{"id": id.String()})
}
