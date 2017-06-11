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



//     BoardUi *boardui=new BoardUi(this);
     GobangBoard *gobangboard=new GobangBoard(this);
     QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation);
     QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
     QObject::connect(ui->boardui,&BoardUi::requestRegret,gobangboard,&GobangBoard::regret);
     QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)));
     QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)));
     QObject::connect(gobangboard,SIGNAL(requestGameover(void)),ui->boardui,SLOT(gameover(void)));
     // ////////////////////
     //I write the SIGNAL(requestSave()) in class::boardui
     //but I did not bound it with the button "save"
     //you may rewrite the signal or add a button
     //same as SIGNAL(requestRead()) and the read act
     //QObject::connect(gobangboard,SIGNAL(requestSave(void)),gobangboard,SLOT(save(void)));
     //QObject::connect(gobangboard,SIGNAL(requestRead(void)),gobangboard,SLOT(read(void)));
     // ////////////////////
     gobangboard->startTimer();

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
