#include "gobangboard.h"
#include "marcros.h"
GobangBoard::GobangBoard(QWidget *parent):QWidget (parent){
    state = INGAME;
    player = BLACK;
    memset(board,0,sizeof(board));
//    for (int i = 0; i < SIZE; i++)
//      for (int j = 0; j < SIZE; j++)
//        board[i][j] = 0;
    emit boardChange(state,player,board);
}

GobangBoard::~GobangBoard(){}

//int GobangBoard::get_State(void){ return state; }
//int GobangBoard::get_Player(void){ return player; }
//int (*GobangBoard::get_Board(void))[SIZE]{ return board; }

// place the chess piece in the target location
// change state & switch player
// return ERROR if any
int GobangBoard::play(int x, int y) {
  if (state == OVER) {
    return ERROR_OVER;
  } else if (board[x][y]) {
    return ERROR_PLACE;
  } else {
    board[x][y] = player;
    record.push_back(x * 100 + y);
    check(x, y);
    emit boardChange(state,player,board);
    return ERROR_NONE;
  }
}

// regret
// return error if there's nothing to regret
// if succeed, change state or player
int GobangBoard::regret(void) {
  if (record.size() == 0)
    return ERROR_REGRET;
  int n = record[record.size() - 1];
  record.pop_back();
  board[n / 100][n % 100] = 0;
  if (state == OVER)
    state = INGAME;
  else {
    if (player == BLACK)
      player = WHITE;
    else
      player = BLACK;
  }
  emit boardChange(state,player,board);
  return 0;
}



// count the number of chess pieces in a row
// UD	->	up down
// LR	->	left right
//LURD	->	left-up right-down
//LDRU	->	left-down right-up
int GobangBoard::countUD(int x, int y) {
  int c = 0;
  int temp = x;
  while ((x >= 0) && (board[x][y] == player)) {
    c++;
    x--;
  }
  x = temp;
  while ((x < SIZE) && (board[x][y] == player)) {
    c++;
    x++;
  }
  return (c - 1);
}

int GobangBoard::countLR(int x, int y) {
  int c = 0;
  int temp = y;
  while ((y >= 0) && (board[x][y] == player)) {
    c++;
    y--;
  }
  y = temp;
  while ((y < SIZE) && (board[x][y] == player)) {
    c++;
    y++;
  }
  return (c - 1);
}

int GobangBoard::countLURD(int x, int y) {
  int c = 0;
  int tempx = x, tempy = y;
  while ((y >= 0) && (x >= 0) && (board[x][y] == player)) {
    c++;
    y--;
    x--;
  }
  x = tempx;
  y = tempy;
  while ((y < SIZE) && (x < SIZE) && (board[x][y] == player)) {
    c++;
    y++;
    x++;
  }
  return (c - 1);
}

int GobangBoard::countLDRU(int x, int y) {
  int c = 0;
  int tempx = x, tempy = y;
  while ((y >= 0) && (x < SIZE) && (board[x][y] == player)) {
    c++;
    y--;
    x++;
  }
  x = tempx;
  y = tempy;
  while ((y < SIZE) && (x >= 0) && (board[x][y] == player)) {
    c++;
    y++;
    x--;
  }
  return (c - 1);
}

// check if this round is over
// switch player
// change state
void GobangBoard::check(int x, int y) {
  int UD = countUD(x, y), LR = countLR(x, y), LURD = countLURD(x, y),
      LDRU = countLDRU(x, y);
  if ((UD >= 5) || (LR >= 5) || (LURD >= 5) || (LDRU >= 5)) {
    state = OVER;
  } else {
    state = INGAME;
    if (player == BLACK)
      player = WHITE;
    else
      player = BLACK;
  }
}

