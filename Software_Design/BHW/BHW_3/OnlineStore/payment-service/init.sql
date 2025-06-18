DROP TABLE IF EXISTS account;

CREATE TABLE account (
    id UUID PRIMARY KEY,
    balance FLOAT NOT NULL DEFAULT 0,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE inbox (
    id            UUID            PRIMARY KEY,
    received_at   TIMESTAMPTZ     NOT NULL DEFAULT now(),
    event_type    TEXT            NOT NULL,
    payload       JSONB           NOT NULL,
    processed_at  TIMESTAMPTZ
);

CREATE TABLE outbox (
    id            UUID            PRIMARY KEY DEFAULT gen_random_uuid(),
    occurred_at   TIMESTAMPTZ     NOT NULL DEFAULT now(),
    event_type    TEXT            NOT NULL,
    payload       JSONB           NOT NULL,
    dispatched_at TIMESTAMPTZ
);