package kafka

import (
	"context"
	"github.com/google/uuid"
	"github.com/segmentio/kafka-go"
	"order-service/internal/domain"
	"order-service/internal/repository"
	"time"
)

func StartConsumer(ctx context.Context, brokers []string, topic, groupID string, inboxRepo *repository.InboxRepo, handler func(domain.InboxMessage) error) {
	r := kafka.NewReader(kafka.ReaderConfig{
		Brokers: brokers,
		Topic:   topic,
		GroupID: groupID,
	})
	go func() {
		for {
			m, err := r.FetchMessage(ctx)
			if err != nil {
				return
			}
			id := uuid.New()
			inboxMsg := domain.InboxMessage{
				ID:         id,
				ReceivedAt: time.Now().UTC(),
				EventType:  m.Topic,
				Payload:    m.Value,
			}
			_ = inboxRepo.SaveIfNotExists(ctx, &inboxMsg)
			stored, _ := inboxRepo.Get(ctx, id)
			if stored.ProcessedAt == nil {
				_ = handler(inboxMsg)
				_ = inboxRepo.MarkProcessed(ctx, id)
			}
			_ = r.CommitMessages(ctx, m)
		}
	}()
}
