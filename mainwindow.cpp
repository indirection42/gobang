#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gobangboard.h"
#include "boardui.h"
#include "client.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
//     client *cli = new client(this);
//     cli->setServer("localhost",39299);//according to the server's IP and port
//     QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),cli,SLOT(sendLocalPlay(int,int)));
//     QObject::connect(cli,SIGNAL(getRemotePlay(int,int)),gobangboard,SLOT(play(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionQuit_Application_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionLocal_PvP_triggered()
{
    if(!this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly)){
        GobangBoard *gobangboard=new GobangBoard(this);
        QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation);
        QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
        QObject::connect(ui->boardui,&BoardUi::requestRegret,gobangboard,&GobangBoard::regret);
        QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)));
        QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)));
        QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)));
        QObject::connect(gobangboard,SIGNAL(requestGameover(int)),ui->boardui,SLOT(gameOver(int)));
        QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)));
        QObject::connect(ui->regretButton,SIGNAL(clicked(void)),ui->boardui,SLOT(regretBinding(void)));
        QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),ui->boardui,SLOT(giveupBinding(void)));
    }
    ui->boardui->newGame(LOCALPVP);
}
