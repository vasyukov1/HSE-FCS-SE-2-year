package infrastructure

import (
	"api-gateway/internal/domain"
	"bytes"
	"context"
	"io"
	"net/http"
	"testing"
)

type mockRoundTripper struct {
	resp *http.Response
	err  error
}

func (m *mockRoundTripper) RoundTrip(req *http.Request) (*http.Response, error) {
	return m.resp, m.err
}

func newTestOrderClient(resp *http.Response, err error) *OrderClient {
	return &OrderClient{
		baseURL: "http://test",
		httpClient: &http.Client{
			Transport: &mockRoundTripper{resp: resp, err: err},
		},
	}
}

func TestOrderClient_Create_Success(t *testing.T) {
	body := `{"id":"1","user_id":"u1","amount":100,"description":"desc","status":"finished","created_at":"2024-01-01T00:00:00Z","updated_at":"2024-01-01T00:00:00Z"}`
	resp := &http.Response{
		StatusCode: http.StatusCreated,
		Body:       io.NopCloser(bytes.NewBufferString(body)),
	}
	client := newTestOrderClient(resp, nil)

	req := domain.OrderRequest{UserID: "u1", Amount: 100, Description: "desc"}
	out, err := client.Create(context.Background(), req)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if out == nil || out.ID != "1" {
		t.Fatalf("unexpected result: %+v", out)
	}
}

func TestOrderClient_Create_HTTPError(t *testing.T) {
	client := newTestOrderClient(nil, io.EOF)
	req := domain.OrderRequest{}
	_, err := client.Create(context.Background(), req)
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}

func TestOrderClient_Create_StatusError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusBadRequest,
		Body:       io.NopCloser(bytes.NewBufferString("fail")),
	}
	client := newTestOrderClient(resp, nil)
	req := domain.OrderRequest{}
	_, err := client.Create(context.Background(), req)
	if err == nil || err.Error() == "" {
		t.Fatal("expected status error, got nil")
	}
}

func TestOrderClient_Create_DecodeError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusCreated,
		Body:       io.NopCloser(bytes.NewBufferString("{not json")),
	}
	client := newTestOrderClient(resp, nil)
	req := domain.OrderRequest{}
	_, err := client.Create(context.Background(), req)
	if err == nil {
		t.Fatal("expected decode error, got nil")
	}
}

func TestOrderClient_List_Success(t *testing.T) {
	body := `[{"id":"1","user_id":"u1","amount":100,"description":"desc","status":"finished","created_at":"2024-01-01T00:00:00Z","updated_at":"2024-01-01T00:00:00Z"}]`
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       io.NopCloser(bytes.NewBufferString(body)),
	}
	client := newTestOrderClient(resp, nil)
	out, err := client.List(context.Background())
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if len(out) != 1 || out[0].ID != "1" {
		t.Fatalf("unexpected result: %+v", out)
	}
}

func TestOrderClient_List_HTTPError(t *testing.T) {
	client := newTestOrderClient(nil, io.EOF)
	_, err := client.List(context.Background())
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}

func TestOrderClient_List_StatusError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusBadGateway,
		Body:       io.NopCloser(bytes.NewBufferString("fail")),
	}
	client := newTestOrderClient(resp, nil)
	_, err := client.List(context.Background())
	if err == nil || err.Error() == "" {
		t.Fatal("expected status error, got nil")
	}
}

func TestOrderClient_List_DecodeError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       io.NopCloser(bytes.NewBufferString("{not json")),
	}
	client := newTestOrderClient(resp, nil)
	_, err := client.List(context.Background())
	if err == nil {
		t.Fatal("expected decode error, got nil")
	}
}

func TestOrderClient_Status_Success(t *testing.T) {
	body := `{"id":"1","status":"finished"}`
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       io.NopCloser(bytes.NewBufferString(body)),
	}
	client := newTestOrderClient(resp, nil)
	out, err := client.Status(context.Background(), "1")
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if out == nil || out.ID != "1" {
		t.Fatalf("unexpected result: %+v", out)
	}
}

func TestOrderClient_Status_HTTPError(t *testing.T) {
	client := newTestOrderClient(nil, io.EOF)
	_, err := client.Status(context.Background(), "1")
	if err == nil {
		t.Fatal("expected error, got nil")
	}
}

func TestOrderClient_Status_StatusError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusNotFound,
		Body:       io.NopCloser(bytes.NewBufferString("fail")),
	}
	client := newTestOrderClient(resp, nil)
	_, err := client.Status(context.Background(), "1")
	if err == nil || err.Error() == "" {
		t.Fatal("expected status error, got nil")
	}
}

func TestOrderClient_Status_DecodeError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       io.NopCloser(bytes.NewBufferString("{not json")),
	}
	client := newTestOrderClient(resp, nil)
	_, err := client.Status(context.Background(), "1")
	if err == nil {
		t.Fatal("expected decode error, got nil")
	}
}
