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
#include <QFileDialog>
#include <QTextStream>
class BoardUi : public QWidget
{
    Q_OBJECT
public:
    explicit BoardUi(QWidget *parent = nullptr);
    ~BoardUi();
signals:
    void requestPlay(int i,int j);
    void requestRegret(int player);
    void requestGiveUp(int player);
    void start();
    void load();
    void requestSetTime(int blackTime, int whiteTime);
    void requestSave(int gameMode);
public slots:
    void newGame(int newGameMode);
    int loadGame(void);
    void updateInformation(int state,int player,int board[SIZE][SIZE],QVector<int> record);
    void gameOver(int winner);
    void regretBinding();
    void giveupBinding();
    void save(void);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int gameMode;
    int localPlayer;
    int stateCopy;
    int playerCopy;
    int boardCopy[SIZE][SIZE];
    QVector<int> recordCopy;
    int moveSignX;
    int moveSignY;
    int addNumber=0;
};

#endif // BOARDUI_H
