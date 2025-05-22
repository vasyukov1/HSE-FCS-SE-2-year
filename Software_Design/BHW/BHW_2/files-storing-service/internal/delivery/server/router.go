package server

import "net/http"

func NewRouter(handler http.Handler) *http.ServeMux {
	mux := http.NewServeMux()
	mux.Handle("/", handler)
	return mux
}
