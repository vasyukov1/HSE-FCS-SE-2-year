package payment

import (
	"bytes"
	"context"
	"io/ioutil"
	"net/http"
	"testing"
	"time"

	"github.com/google/uuid"
)

type mockRoundTripper struct {
	resp *http.Response
	err  error
}

func (m *mockRoundTripper) RoundTrip(req *http.Request) (*http.Response, error) {
	return m.resp, m.err
}

func newTestClient(resp *http.Response, err error) *PaymentClient {
	return &PaymentClient{
		BaseURL: "http://test",
		Client: &http.Client{
			Timeout:   1 * time.Second,
			Transport: &mockRoundTripper{resp: resp, err: err},
		},
	}
}

func TestWithdraw_Success(t *testing.T) {
	body := `{"account_id":"acc-1","balance":900.0}`
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       ioutil.NopCloser(bytes.NewBufferString(body)),
	}
	client := newTestClient(resp, nil)

	id := uuid.New()
	result, err := client.Withdraw(context.Background(), id, 100)
	if err != nil {
		t.Fatalf("expected no error, got %v", err)
	}
	if result == nil || result.Balance != 900.0 {
		t.Fatalf("unexpected result: %+v", result)
	}
}

func TestWithdraw_InsufficientFunds(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusPaymentRequired,
		Body:       ioutil.NopCloser(bytes.NewBufferString("")),
	}
	client := newTestClient(resp, nil)

	id := uuid.New()
	_, err := client.Withdraw(context.Background(), id, 100)
	if err == nil || err.Error() != "insufficient funds" {
		t.Fatalf("expected insufficient funds error, got %v", err)
	}
}

func TestWithdraw_PaymentServiceError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusInternalServerError,
		Status:     "500 Internal Server Error",
		Body:       ioutil.NopCloser(bytes.NewBufferString("")),
	}
	client := newTestClient(resp, nil)

	id := uuid.New()
	_, err := client.Withdraw(context.Background(), id, 100)
	if err == nil || err.Error() != "payment service error: 500 Internal Server Error" {
		t.Fatalf("expected payment service error, got %v", err)
	}
}

func TestWithdraw_DecodeError(t *testing.T) {
	resp := &http.Response{
		StatusCode: http.StatusOK,
		Body:       ioutil.NopCloser(bytes.NewBufferString("{not json")),
	}
	client := newTestClient(resp, nil)

	id := uuid.New()
	_, err := client.Withdraw(context.Background(), id, 100)
	if err == nil {
		t.Fatal("expected decode error, got nil")
	}
}
