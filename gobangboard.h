#ifndef GOBANGBOARD_H
#define GOBANGBOARD_H
#include "marcros.h"
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
class GobangBoard:public QWidget
{
    Q_OBJECT
public:
    explicit GobangBoard(QWidget *parent=0);
    ~GobangBoard();
//    int get_State();
//    int get_Player();
//    int (*get_Board())[SIZE];
signals:
    void boardChange(int state,int player,int board[SIZE][SIZE],  QVector<int> record);
    void blackTimeChange(int blackTimer);
    void whiteTimeChange(int whiteTimer);
    void requestGameover(int winner);    //emit the signal when the game is over
public slots:
    int play(int x, int y);
    int regret(int regreter);
    void giveup(int loser);
    void start();
    int save(int gamemode);     //can call it at any time
    //int read();     //have to clean the board then call it
    void loadBoard(int b, int w,QVector<int>);
private slots:
    void changePlayerTimer();
private:
    int state;
    int player;
    int board[SIZE][SIZE];
    QVector<int> record;
    int blackTimer;
    int whiteTimer;
    int countUD(int x,int y);
    int countLR(int x,int y);
    int countLURD(int x,int y);
    int countLDRU(int x, int y);
    void check(int x, int y);
};

#endif // GOBANGBOARD_H
