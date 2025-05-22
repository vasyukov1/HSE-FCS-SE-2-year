FROM golang:1.24 AS builder

WORKDIR /BHW_2

COPY go.mod go.sum ./
RUN go mod tidy

COPY . .

RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -o file-analysis-service ./cmd/app

FROM alpine:latest AS runner

WORKDIR /

COPY --from=builder /BHW_2/file-analysis-service .

EXPOSE 8080

CMD ["./file-analysis-service"]