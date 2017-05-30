#ifndef GOBANGBOARD_H
#define GOBANGBOARD_H
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include <QLineF>
class GobangBoard:public QWidget
{
    Q_OBJECT
public:
    explicit GobangBoard(QWidget *parent=0);
//    GobangBoard(QWidget *parent=0)
    ~GobangBoard();
//    int get_State();
//    int get_Player();
//    int (*get_Board())[15];
    int play(int x, int y);
    int regret();
//    int save(std::string filename);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int state;
    int player;
    int board[15][15];
    QVector<int> record;
    int countUD(int x,int y);
    int countLR(int x,int y);
    int countLURD(int x,int y);
    int countLDRU(int x, int y);
    void check(int x, int y);
};

#endif // GOBANGBOARD_H
