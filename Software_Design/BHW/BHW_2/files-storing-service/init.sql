CREATE TABLE files (
    id UUID PRIMARY KEY,
    name TEXT NOT NULL,
    hash TEXT NOT NULL,
    location TEXT NOT NULL
);