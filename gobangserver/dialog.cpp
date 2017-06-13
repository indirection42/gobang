#include "dialog.h"
#include "ui_dialog.h"
#include <qmessagebox.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    playernum = 0;

    tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::Any,33333)) {
        QMessageBox::critical(this, tr("Gobang Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpserver->errorString()));
        close();
        return;
    }
    connect(tcpserver, &QTcpServer::newConnection, this, &Dialog::newPlayerConnected);
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Gobang Client now.")
                         .arg(ipAddress).arg(tcpserver->serverPort()));
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::newPlayerConnected()
{
    QByteArray block;


    if(!playernum)
    {
        player_WHITE = tcpserver->nextPendingConnection();
        connect(player_WHITE, &QAbstractSocket::disconnected,
                this, &Dialog::whitedisconnected);
        connect(player_WHITE, &QIODevice::readyRead, this, &Dialog::dataFromWHITE);
        block.push_back(WAIT);
        player_WHITE->write(block);
        qDebug()<<"player_white connected.";

        playernum = 1;
    }
    else if(playernum==1)
    {
        player_BLACK = tcpserver->nextPendingConnection();
        connect(player_BLACK, &QAbstractSocket::disconnected,
                this, &Dialog::blackdisconnected);
        connect(player_BLACK, &QIODevice::readyRead, this, &Dialog::dataFromBLACK);
        block.push_back(READY);
        player_BLACK->write(block);
        player_WHITE->write(block);
        qDebug()<<"player_black connected.Game starts!";

        playernum = 2;
    }
}

void Dialog::dataFromWHITE()
{
    //read WHITE's input

    QByteArray inblock= player_WHITE->readAll();
    qDebug()<<"player_white:"<<inblock;

    //write to BLACK
    player_BLACK->write(inblock);
}

void Dialog::dataFromBLACK()
{
    //read BLACK's input

    QByteArray inblock= player_BLACK->readAll();
    qDebug()<<"player_black:"<<inblock;

    //write to WHITE
    player_WHITE->write(inblock);
}
void Dialog::whitedisconnected()
{
    player_BLACK->write("O",1);
    qDebug()<<"player_white disconnected,reinitializing server.";
    disconnect(player_BLACK, &QAbstractSocket::disconnected,
            this, &Dialog::blackdisconnected);
    player_BLACK->close();
    delete player_WHITE;
    player_WHITE = nullptr;
    delete player_BLACK;
    player_BLACK = nullptr;
    playernum = 0;
}
void Dialog::blackdisconnected()
{
    player_WHITE->write("O",1);
    qDebug()<<"player_black disconnected,reinitializing server.";
    disconnect(player_WHITE, &QAbstractSocket::disconnected,
            this, &Dialog::whitedisconnected);
    player_WHITE->close();
    delete player_WHITE;
    player_WHITE = nullptr;
    delete player_BLACK;
    player_BLACK = nullptr;
    playernum = 0;
}
