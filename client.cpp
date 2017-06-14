#include "client.h"
#include <QMessageBox>
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
        QMessageBox::information(static_cast<QWidget*>(parent()), tr("Gobang Client"),
                                 tr("The host was not found. Please check the host name and port settings."));

        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(static_cast<QWidget*>(parent()), tr("Gobang Client"),tr("The connection was refused by the peer. "
                                                              "Make sure the gobang server is running, "
                                                              "and check that the host name and port "
                                                              "settings are correct."));
        break;
    default:
        QMessageBox::information(static_cast<QWidget*>(parent()), tr("Gobang Client"),tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
        break;
    }

}
void client::sendLocalPlay(int x, int y)
{
    if(!socket->isValid())
        return;
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

    switch (status) {
    case PLAY:
        getRemotePlay(x,y);
        break;
    case READY:
        RemotePlayerReady(player);
        break;
    case WAIT:
        player = ONLINEPVPWHITE;
        waitForReply();
        break;
    case DISCONNECT:
        RemotePlayerdisconnected();
        break;
    case REGRET:
        regretRequest();
        break;
    case AGREE:
        if(player==ONLINEPVPBLACK)
            excuteRegret(BLACK);
        else
            excuteRegret(WHITE);
        break;
    case GIVEUP:
        if(player==ONLINEPVPBLACK)
            excuteGiveup(WHITE);
        else
            excuteGiveup(BLACK);
        break;
    default:
        break;
    }

 }
void client::sendRegretRequest()
{
    if(!socket->isValid())
        return;
    char status = REGRET;
    QByteArray data;

    data.push_back(status);

    socket->write(data);
}
void client::regretRequest()
{
    if(QMessageBox::Yes==QMessageBox::question(static_cast<QWidget*>(parent()),tr("Regret request"),tr("Another Player want to regret one step, do you agree?")))
    {
        if(player==ONLINEPVPBLACK)
            excuteRegret(WHITE);
        else
            excuteRegret(BLACK);

        char status = AGREE;
        QByteArray data;

        data.push_back(status);

        socket->write(data);
    }
    else
    {
        char status = DISAGREE;
        QByteArray data;

        data.push_back(status);

        socket->write(data);
    }

}
void client::sendGiveupRequest()
{
    if(player==ONLINEPVPBLACK)
        excuteGiveup(BLACK);
    else
        excuteGiveup(WHITE);

    char status = GIVEUP;
    QByteArray data;

    data.push_back(status);

    socket->write(data);
}
