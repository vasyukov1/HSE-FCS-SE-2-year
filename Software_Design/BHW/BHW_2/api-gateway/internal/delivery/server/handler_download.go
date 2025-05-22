package server

import (
	"io"
	"net/http"
)

// handleDownload godoc
// @Summary Download file
// @Description Downloads the file with the given ID from the file storage service
// @Tags files
// @Produce octet-stream
// @Param id query string true "File ID"
// @Success 200 {file} file "File content"
// @Failure 400 {object} map[string]string "Missing or invalid ID"
// @Failure 502 {object} map[string]string "Failed to fetch file from storage"
// @Router /download [get]
func (h *Handler) handleDownload(w http.ResponseWriter, r *http.Request) {
	// Parse ID
	id := r.URL.Query().Get("id")
	if id == "" {
		http.Error(w, "Missing 'id' parameter", http.StatusBadRequest)
		return
	}

	// Get file from Files Storing Service
	file, err := h.fileService.Get(r.Context(), id)
	if err != nil {
		http.Error(w, "Error to get file from storage: "+err.Error(), http.StatusBadGateway)
		return
	}
	defer file.Content.Close()

	// Forward response
	w.WriteHeader(http.StatusOK)
	io.Copy(w, file.Content)
}
