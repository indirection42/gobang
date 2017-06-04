#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork>
#define PLAY 'P'
#define READY 'R'
#define WAIT 'W'
#define OVER 'O'

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void newPlayerConnected();
    void dataFromWHITE();
    void dataFromBLACK();
    void whitedisconnected();
    void blackdisconnected();

private:
    Ui::Dialog *ui;
    QTcpServer *tcpserver;
    QTcpSocket *player_BLACK;
    QTcpSocket *player_WHITE;
    int playernum;
};

#endif // DIALOG_H
