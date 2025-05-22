package server

import (
	"api-gateway/internal/domain"
	"encoding/json"
	"net/http"
	"path/filepath"
)

// handleUpload godoc
// @Summary Upload file
// @Description Uploads a .txt file to the file storage service
// @Tags files
// @Accept multipart/form-data
// @Produce json
// @Param file formData file true "Text file (.txt) to upload"
// @Success 200 {object} map[string]string "File uploaded successfully with returned ID"
// @Failure 400 {object} map[string]string "Invalid input or file"
// @Failure 502 {object} map[string]string "Failed to forward file to storage service"
// @Router /upload [post]
func (h *Handler) handleUpload(w http.ResponseWriter, r *http.Request) {
	// Parse multipart form
	if err := r.ParseMultipartForm(10 << 20); err != nil {
		http.Error(w, "Failed to parse multipart form", http.StatusBadRequest)
		return
	}

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

	// Send file to Files Storing Service
	domainFile := &domain.File{
		Name:    header.Filename,
		Content: file,
	}
	id, err := h.fileService.Store(r.Context(), domainFile)
	if err != nil {
		http.Error(w, "Error to forward file", http.StatusBadGateway)
		return
	}

	// Forward response
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{"id": id})
}
