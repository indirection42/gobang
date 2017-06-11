#ifndef MARCROS_H
#define MARCROS_H
//SIZE
#define SIZE 15

//GameMode
#define LOCALPVP 6651
#define LOCALPVC 6652
#define ONLINEPVP 6653

// PLAYER
#define BLACK 6661
#define WHITE 6662
#define BOTH 6663
// STATE
#define OVER 6671
#define INGAME 6672
#define IDLE 6673

// ERROR
#define ERROR_NONE 6680 // no error
#define ERROR_PLACE 6681 // can not play cause another chess piece is on the position
#define ERROR_OVER 6682   // can not play cause this round is over
#define ERROR_IDLE 6683   //can not play cause the game is idle
#define ERROR_SAVE 6684   // can not save file
#define ERROR_REGRET 6685 // nothing to regret
#define ERROR_READ 6686 //can not read saving

#endif // MARCROS_H
