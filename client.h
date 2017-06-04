#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>

#define PLAY 'P'
#define READY 'R'
#define WAIT 'W'
#define DISCONNECT 'O'

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    void setServer(QString servername,quint16 port);

signals:
    void RemotePlayerReady();//there is a remote player who is ready to play
    void getRemotePlay(int x,int y);//the remote player played
    void waitForReply();//When received this signal, the window should lock the input
    void RemotePlayerdisconnected();//when the other client disconnected, this game is over
public slots:
    void sendLocalPlay(int x, int y);//use this to send a play

private slots:
    void receivedData();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
signals:
};

#endif // CLIENT_H
