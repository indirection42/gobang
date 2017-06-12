#include "client.h"

client::client(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket)
{
    connect(socket, &QIODevice::readyRead, this, &client::receivedData);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &client::displayError);
    player = ONLINEPVPBLACK;
}
void client::setServer(QString servername, quint16 port)
{
    socket->connectToHost(servername,port);
}
void client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<tr("The host was not found. Please check the host name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<tr("The connection was refused by the peer. "
                  "Make sure the fortune server is running, "
                  "and check that the host name and port "
                  "settings are correct.");
        break;
    default:
        qDebug()<<tr("The following error occurred: %1.")
                  .arg(socket->errorString());
        break;
    }

}
void client::sendLocalPlay(int x, int y)
{
    char xc=x;
    char yc=y;
    char status = PLAY;
    QByteArray data;

    data.push_back(status);
    data.push_back(xc);
    data.push_back(yc);

    socket->write(data);
    this->waitForReply();
}
void client::receivedData()
{
    char x,y;
    char status;
    //read input
    QByteArray inblock;

    inblock = socket->readAll();

    status = inblock.data()[0];
    x = inblock.data()[1];
    y = inblock.data()[2];

    if(status==PLAY)
     {
         getRemotePlay(x,y);
     }
     else if(status==READY)
     {
         RemotePlayerReady(player);
     }
     else if(status==WAIT)
     {
         player = ONLINEPVPWHITE;
         waitForReply();
     }
     else if(status==DISCONNECT)
     {
         RemotePlayerdisconnected();
     }
 }
