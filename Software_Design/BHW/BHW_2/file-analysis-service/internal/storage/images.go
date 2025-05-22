package storage

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
)

type FSImageSaver struct {
	Dir string
}

func NewFSImageSaver(dir string) *FSImageSaver {
	return &FSImageSaver{Dir: dir}
}

func (s *FSImageSaver) Save(id string, data []byte) (string, error) {
	// Create directory
	if err := os.MkdirAll(s.Dir, 0755); err != nil {
		return "", err
	}

	// Create file
	path := filepath.Join(s.Dir, fmt.Sprintf("%s.png", id))
	f, err := os.Create(path)
	if err != nil {
		return "", err
	}
	defer f.Close()

	// Copy data
	_, err = io.Copy(f, bytes.NewReader(data))
	return path, err
}
