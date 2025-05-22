package storage

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
)

type FileSaver struct {
	Dir string
}

func NewFileSaver(dir string) *FileSaver {
	return &FileSaver{Dir: dir}
}

func (c *FileSaver) Save(id string, data []byte) (string, error) {
	// Create directory
	if err := os.MkdirAll(c.Dir, 0755); err != nil {
		log.Printf("MkdirAll failed: %v", err)
		return "", err
	}

	// Create file
	path := filepath.Join(c.Dir, fmt.Sprintf("%s.txt", id))
	f, err := os.Create(path)
	if err != nil {
		log.Printf("Create failed: %v", err)
		return "", err
	}
	defer f.Close()

	// Copy file data
	if _, err = io.Copy(f, bytes.NewReader(data)); err != nil {
		log.Printf("Write failed: %v", err)
		return "", err
	}

	log.Printf("Successfully saved %d bytes to %s", len(data), path)
	return path, nil
}

func (c *FileSaver) Read(location string) ([]byte, error) {
	return os.ReadFile(location)
}
