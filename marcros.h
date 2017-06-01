#ifndef MARCROS_H
#define MARCROS_H
//SIZE
#define SIZE 15

// PLAYER
#define BLACK 6661
#define WHITE 6662

// STATE
#define OVER 6671
#define INGAME 6672

// ERROR
#define ERROR_NONE 6680 // no error
#define ERROR_PLACE 6681 // can not play cause another chess piece is on the position
#define ERROR_OVER 6682   // can not play cause this round is over
#define ERROR_SAVE 6683   // can not save file
#define ERROR_REGRET 6684 // nothing to regret
#endif // MARCROS_H
