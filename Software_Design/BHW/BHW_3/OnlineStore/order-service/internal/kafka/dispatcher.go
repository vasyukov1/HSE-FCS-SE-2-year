package kafka

import (
	"context"
	"time"

	"order-service/internal/repository"
)

type OutboxDispatcher struct {
	repo      *repository.OutboxRepo
	producer  *Producer
	interval  time.Duration
	batchSize int
}

func NewOutboxDispatcher(repo *repository.OutboxRepo, producer *Producer, interval time.Duration, batchSize int) *OutboxDispatcher {
	return &OutboxDispatcher{repo, producer, interval, batchSize}
}

func (d *OutboxDispatcher) Start(ctx context.Context) {
	ticker := time.NewTicker(d.interval)
	defer ticker.Stop()

	for {
		select {
		case <-ctx.Done():
			return
		case <-ticker.C:
			msgs, err := d.repo.FetchUnsent(ctx, d.batchSize)
			if err != nil {
				continue
			}
			for _, m := range msgs {
				if err := d.producer.SendRaw(ctx, []byte(m.EventType), m.Payload); err != nil {
					break
				}
				_ = d.repo.MarkDispatched(ctx, m.ID)
			}
		}
	}
}
