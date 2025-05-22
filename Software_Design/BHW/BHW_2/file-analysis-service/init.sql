DROP TABLE IF EXISTS analysis;

CREATE TABLE analysis (
    id UUID PRIMARY KEY,
    location TEXT NOT NULL,
    word_count INTEGER,
    paragraph_count INTEGER,
    character_count INTEGER
);