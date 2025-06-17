FROM golang:1.24 AS builder

WORKDIR /OnlineStore

COPY go.mod go.sum ./
RUN go mod tidy

COPY . .

RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -o order-service ./cmd/server

FROM alpine:latest AS runner

WORKDIR /

COPY --from=builder /OnlineStore/order-service .

EXPOSE 8080

CMD ["./order-service"]