#ifndef COMMON_H
#define COMMON_H

#define MAX_PLAYERS 10
#define MSG_KEY 0x1234
#define SEM_KEY 0x5678

typedef enum { ROCK, SCISSORS, PAPER } Move;

typedef struct {
    long mtype;
    int sender_id;
    int receiver_id;
    Move move;
} Message;

typedef struct {
    long mtype;
    Move my_move;
    Move opponent_move;
} ResultMessage;

#endif
