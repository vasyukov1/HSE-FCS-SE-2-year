package kafka

import (
	"context"

	"github.com/segmentio/kafka-go"
)

type Producer struct {
	writer *kafka.Writer
}

func NewProducer(brokers []string, topic string) *Producer {
	return &Producer{writer: &kafka.Writer{
		Addr:  kafka.TCP(brokers...),
		Topic: topic,
	}}
}

func (p *Producer) SendRaw(ctx context.Context, key, payload []byte) error {
	return p.writer.WriteMessages(ctx, kafka.Message{
		Key:   key,
		Value: payload,
	})
}

func (p *Producer) Close() error {
	return p.writer.Close()
}
