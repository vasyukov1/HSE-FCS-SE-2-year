#ifndef COMMON_H
#define COMMON_H

#define MAX_PLAYERS 10
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

typedef enum { ROCK, SCISSORS, PAPER } Move;

typedef struct {
    int moves[MAX_PLAYERS];
    int scores[MAX_PLAYERS];
    int ready[MAX_PLAYERS][MAX_PLAYERS];
} SharedData;

enum {
    SEM_MUTEX = 0,
    SEM_READY_BASE = 1
};

#endif
