#ifndef GOBANGBOARD_H
#define GOBANGBOARD_H
#include "marcros.h"
#include <QWidget>
#include <QVector>
#include <QTimer>
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
    void boardChange(int state,int player,int board[SIZE][SIZE]);
public slots:
    int play(int x, int y);
    int regret();
private:
    int state;
    int player;
    int board[SIZE][SIZE];
    QVector<int> record;
    int countUD(int x,int y);
    int countLR(int x,int y);
    int countLURD(int x,int y);
    int countLDRU(int x, int y);
    void check(int x, int y);
};

#endif // GOBANGBOARD_H
