package delivery

import (
	"context"
	"encoding/json"
	"errors"
	"net/http"
	"net/http/httptest"
	"payment-service/internal/infrastructure"
	"testing"

	"github.com/google/uuid"
	"payment-service/internal/domain"
)

type stubStorage struct {
	balance float64
	err     error
}

func (s *stubStorage) Save(ctx context.Context, id uuid.UUID) error {
	panic("не используется")
}
func (s *stubStorage) Get(ctx context.Context, id uuid.UUID) (float64, error) {
	return s.balance, s.err
}
func (s *stubStorage) UpdateBalance(ctx context.Context, id uuid.UUID, delta float64) (float64, error) {
	panic("не используется")
}
func (s *stubStorage) Withdraw(ctx context.Context, id uuid.UUID, amount float64) (float64, error) {
	panic("не используется")
}

func makeHandler(balance float64, err error) *Handler {
	st := &stubStorage{balance: balance, err: err}
	srv := infrastructure.NewServer(st)
	return NewHandler(srv)
}

func TestHandleBalance_Success(t *testing.T) {
	id := uuid.New()
	expected := &domain.BalanceResponse{
		AccountID: id.String(),
		Balance:   123.45,
	}

	h := makeHandler(expected.Balance, nil)
	req := httptest.NewRequest(http.MethodGet, "/accounts/"+id.String()+"/balance", nil)
	w := httptest.NewRecorder()

	h.handleBalance(w, req)

	res := w.Result()
	defer res.Body.Close()

	if res.StatusCode != http.StatusOK {
		t.Fatalf("ожидали 200 OK, получили %d", res.StatusCode)
	}
	if ct := res.Header.Get("Content-Type"); ct != "application/json" {
		t.Errorf("ожидали Content-Type application/json, получили %q", ct)
	}

	var got domain.BalanceResponse
	if err := json.NewDecoder(res.Body).Decode(&got); err != nil {
		t.Fatalf("не смогли декодировать JSON: %v", err)
	}
	if got != *expected {
		t.Errorf("ожидали тело %+v, получили %+v", *expected, got)
	}
}

func TestHandleBalance_NotFound(t *testing.T) {
	h := makeHandler(0, nil)
	req := httptest.NewRequest(http.MethodGet, "/foo/bar", nil)
	w := httptest.NewRecorder()

	h.handleBalance(w, req)

	res := w.Result()
	defer res.Body.Close()

	if res.StatusCode != http.StatusNotFound {
		t.Fatalf("ожидали 404 Not Found, получили %d", res.StatusCode)
	}
}

func TestHandleBalance_InvalidUUID(t *testing.T) {
	h := makeHandler(0, nil)
	req := httptest.NewRequest(http.MethodGet, "/accounts/not-a-uuid/balance", nil)
	w := httptest.NewRecorder()

	h.handleBalance(w, req)

	res := w.Result()
	defer res.Body.Close()

	if res.StatusCode != http.StatusBadRequest {
		t.Fatalf("ожидали 400 Bad Request, получили %d", res.StatusCode)
	}
}

func TestHandleBalance_ServiceError(t *testing.T) {
	id := uuid.New()
	h := makeHandler(0, errors.New("db error"))

	req := httptest.NewRequest(http.MethodGet, "/accounts/"+id.String()+"/balance", nil)
	w := httptest.NewRecorder()

	h.handleBalance(w, req)

	res := w.Result()
	defer res.Body.Close()

	if res.StatusCode != http.StatusInternalServerError {
		t.Fatalf("ожидали 500 Internal Server Error, получили %d", res.StatusCode)
	}
}
