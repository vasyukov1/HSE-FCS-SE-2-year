package kafka

import (
	"context"
	"encoding/json"
	"github.com/segmentio/kafka-go"
	"order-service/internal/domain"
)

type Producer struct {
	writer *kafka.Writer
	topic  string
}

func NewProducer(brokers []string, topic string) *Producer {
	return &Producer{
		writer: &kafka.Writer{
			Addr:     kafka.TCP(brokers...),
			Topic:    topic,
			Balancer: &kafka.LeastBytes{},
		},
		topic: topic,
	}
}

func (p *Producer) SendOrderCreated(ctx context.Context, event domain.OrderCreatedEvent) error {
	value, err := json.Marshal(event)
	if err != nil {
		return err
	}
	msg := kafka.Message{
		Value: value,
	}
	return p.writer.WriteMessages(ctx, msg)
}

func (p *Producer) SendRaw(ctx context.Context, key, payload []byte) error {
	msg := kafka.Message{
		Key:   key,
		Value: payload,
	}
	return p.writer.WriteMessages(ctx, msg)
}

func (p *Producer) Close() error {
	return p.writer.Close()
}
