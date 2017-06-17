#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include "marcros.h"


class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    void setServer(QString servername,quint16 port);

signals:
    void RemotePlayerReady(int GameMode);//there is a remote player who is ready to play, and you should start according to the game mode
    void getRemotePlay(int x,int y);//the remote player played
    void waitForReply();//When received this signal, the window should lock the input
    void RemotePlayerdisconnected();//when the other client disconnected, this game is over
    void excuteRegret(int regreter);//excute the regret operation
    void excuteGiveup(int giveuper);//excute the give up operation

public slots:
    void sendLocalPlay(int x, int y);//use this to send a play
    void sendRegretRequest();//use this to send a regret
    void sendGiveupRequest();//use this to send a give up request
private slots:
    void receivedData();
    void displayError(QAbstractSocket::SocketError socketError);
    void remoteDisconnected();

private:
    QTcpSocket *socket;
    int player;

    void regretRequest();//remote player want to regret
};

#endif // CLIENT_H
