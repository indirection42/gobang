#ifndef BOARDUI_H
#define BOARDUI_H
#include "marcros.h"
#include <QWidget>
#include <QVector>
#include <QLine>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include <QInputDialog>
#include <QLineEdit>
class BoardUi : public QWidget
{
    Q_OBJECT
public:
    explicit BoardUi(QWidget *parent = nullptr);
    ~BoardUi();
signals:
    void requestPlay(int i,int j);
    void requestRegret();
    void requestSave();
    void requestRead();
public slots:
    void updateInformation(int state,int player,int board[SIZE][SIZE],QVector<int> r);
    void gameover();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int stateCopy;
    int playerCopy;
    int boardCopy[SIZE][SIZE];
    int moveSignX;
    int moveSignY;
    int addNumber;
    QVector<int> record;
};

#endif // BOARDUI_H
