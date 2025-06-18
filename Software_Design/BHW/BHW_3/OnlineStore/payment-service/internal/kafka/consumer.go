package kafka

import (
	"context"
	"encoding/json"
	"github.com/jackc/pgx/v5"
	"github.com/segmentio/kafka-go"
	"payment-service/internal/infrastructure"
	"time"

	"github.com/google/uuid"
	"payment-service/internal/domain"
	"payment-service/internal/repository"
)

func StartConsumer(
	ctx context.Context,
	brokers []string,
	topic, groupID string,
	dbConn *pgx.Conn,
	server *infrastructure.Server,
	inboxRepo *repository.InboxRepo,
	outboxRepo *repository.OutboxRepo,
	producer *Producer,
) {
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

			tx, err := dbConn.Begin(ctx)
			if err != nil {
				break
			}

			inboxMsg := &domain.InboxMessage{
				ID:         uuid.New(),
				ReceivedAt: time.Now().UTC(),
				EventType:  m.Topic,
				Payload:    m.Value,
			}
			if err := inboxRepo.SaveIfNotExists(ctx, tx, inboxMsg); err != nil {
				tx.Rollback(ctx)
				break
			}

			var evt domain.OrderCreatedEvent
			if err := json.Unmarshal(m.Value, &evt); err != nil {
				tx.Rollback(ctx)
				break
			}

			_, err = server.Withdraw(ctx, domain.WithdrawRequest{
				AccountID: uuid.MustParse(evt.UserID),
				Amount:    evt.Amount,
			})

			var outEvtType string
			var outPayload []byte
			if err == nil {
				ps := domain.PaymentSucceededEvent{OrderID: evt.OrderID, Amount: evt.Amount}
				outEvtType = "PaymentSucceeded"
				outPayload, _ = json.Marshal(ps)
			} else {
				pf := domain.PaymentFailedEvent{OrderID: evt.OrderID, Reason: err.Error()}
				outEvtType = "PaymentFailed"
				outPayload, _ = json.Marshal(pf)
			}

			outMsg := &domain.OutboxMessage{
				ID:         uuid.New(),
				OccurredAt: time.Now().UTC(),
				EventType:  outEvtType,
				Payload:    outPayload,
			}
			if err := outboxRepo.Save(ctx, tx, outMsg); err != nil {
				tx.Rollback(ctx)
				break
			}

			if err := tx.Commit(ctx); err != nil {
				break
			}

			r.CommitMessages(ctx, m)
		}
	}()
}
