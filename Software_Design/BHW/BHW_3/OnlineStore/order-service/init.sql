DROP TABLE IF EXISTS orders;

CREATE TABLE orders (
    id UUID PRIMARY KEY,
    user_id UUID NOT NULL,
    amount FLOAT NOT NULL DEFAULT 0,
    description TEXT,
    status TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE outbox (
    id            UUID            PRIMARY KEY DEFAULT gen_random_uuid(),
    occurred_at   TIMESTAMPTZ     NOT NULL DEFAULT now(),
    event_type    TEXT            NOT NULL,
    payload       JSONB           NOT NULL,
    dispatched_at TIMESTAMPTZ
);

CREATE TABLE inbox (
    id            UUID            PRIMARY KEY,
    received_at   TIMESTAMPTZ     NOT NULL DEFAULT now(),
    event_type    TEXT            NOT NULL,
    payload       JSONB           NOT NULL,
    processed_at  TIMESTAMPTZ
);