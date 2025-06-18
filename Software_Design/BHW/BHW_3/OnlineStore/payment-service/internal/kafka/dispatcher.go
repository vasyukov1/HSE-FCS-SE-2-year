package kafka

import (
	"context"
	"time"

	"payment-service/internal/repository"
)

type OutboxDispatcher struct {
	repo      *repository.OutboxRepo
	producer  *Producer
	interval  time.Duration
	batchSize int
}

func NewOutboxDispatcher(repo *repository.OutboxRepo, prod *Producer, interval time.Duration, batchSize int) *OutboxDispatcher {
	return &OutboxDispatcher{repo, prod, interval, batchSize}
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
				d.repo.MarkDispatched(ctx, m.ID)
			}
		}
	}
}
