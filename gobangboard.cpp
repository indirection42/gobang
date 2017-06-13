#include "gobangboard.h"
#include "marcros.h"
GobangBoard::GobangBoard(QWidget *parent):QWidget (parent),blackTimer(1800),whiteTimer(1800){
    state = IDLE;
    player = BLACK;
    memset(board,0,sizeof(board));
    record={};
}

GobangBoard::~GobangBoard(){}

// place the chess piece in the target location
// change state & switch player
// return ERROR if any
int GobangBoard::play(int x, int y) {
  if (state == OVER ) {
    return ERROR_OVER;
  } else if (state==IDLE){
      return ERROR_IDLE;
  }
  else if (board[x][y]) {
    return ERROR_PLACE;
  } else{
    board[x][y] = player;
    record.push_back(x * 100 + y);
    check(x, y);
    emit boardChange(state,player,board,record);
    if(state==OVER){
        emit requestGameover(player);
    }
    return ERROR_NONE;
  }
}


// regret
// return error if there's nothing to regret
// if succeed, change state or player
int GobangBoard::regret(int regreter) {
  if (state==IDLE){
      return ERROR_IDLE;
  }
  if (record.size() == 0)
    return ERROR_REGRET;
  int times=1;
  if(regreter==player){times=2;}
  for(int i=0;i<times;i++){
      int n = record[record.size() - 1];
      record.pop_back();
      board[n / 100][n % 100] = 0;
      if (state == OVER)
        state = INGAME;
      if (player == BLACK)
          player = WHITE;
      else
          player = BLACK;
  }
  emit boardChange(state,player,board,record);
  return 0;
}

void GobangBoard::giveup(int loser)
{
    if(state!=INGAME)
        return ;
    state=OVER;
    int winner;
    if(loser==BLACK)
        winner=WHITE;
    else if(loser==WHITE)
        winner=BLACK;
    else{
        if(player==BLACK)
            winner=WHITE;
        else
            winner=BLACK;
    }
    emit boardChange(state,player,board,record);
    emit requestGameover(winner);
}

void GobangBoard::start(){
    state=INGAME;
    player = BLACK;
    memset(board,0,sizeof(board));
    blackTimer=1800;
    whiteTimer=1800;
    record={};
    emit boardChange(state,player,board,record);
    emit blackTimeChange(blackTimer);
    emit whiteTimeChange(whiteTimer);
    if(!this->findChild<QTimer *>(QString(),Qt::FindDirectChildrenOnly)){
    QTimer *secTimer=new QTimer(this);
    QObject::connect(secTimer,&QTimer::timeout,this,&GobangBoard::changePlayerTimer);
    secTimer->start(1000);}
}
void GobangBoard::changePlayerTimer(){
    if(state==INGAME){
        if(player==BLACK){
            blackTimer--;
            emit blackTimeChange(blackTimer);
        }
        else{
            whiteTimer--;
            emit whiteTimeChange(whiteTimer);
        }
    }
}
 void GobangBoard::loadBoard(int b, int w,QVector<int> record)
{
    blackTimer = b;
    whiteTimer = w;
    for(auto i:record){
        play(i/100,i%100);
    }
    emit blackTimeChange(blackTimer);
    emit whiteTimeChange(whiteTimer);
}

int GobangBoard::save(int gamemode)
{
    QString path = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),tr("save files (*.save)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("ERROR"), tr("Illegal file name."));
        return ERROR_SAVE;
    } else
    {
        //path = path + ".save";
        QFile file(path);
        file.open(QFile::WriteOnly);
        if(!file.isOpen())
        {
            printf( "nope\n");
            return ERROR_SAVE;
        }
        QTextStream out(&file);
        out << gamemode << ' ' << blackTimer << ' ' << whiteTimer;
        for(int i = 0;i<record.size();i++)
        {
            out << ' ' << record[i];
        }
        file.close();
        return ERROR_NONE;
    }
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



