#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gobangboard.h"
#include "boardui.h"
#include "GobangAI.h"
#include "client.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

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
    GobangBoard *gobangboard;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
        //gobangboard >> board->ui
        QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation);
        QObject::connect(gobangboard,SIGNAL(requestGameover(int)),ui->boardui,SLOT(gameOver(int)));
        // board->ui >> gobangboard
        QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)));
        QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
        QObject::connect(ui->boardui,SIGNAL(requestRegret(int)),gobangboard,SLOT(regret(int)));
        QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)));
        QObject::connect(ui->boardui,SIGNAL(requestLoadBoard(int,int,QVector<int>)),gobangboard,SLOT(loadBoard(int,int,QVector<int>)));
        QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)));
        //gobangboard >> ui->other_widgets
        QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)));
        QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)));
        //ui->buttons >> ui->boardui
        QObject::connect(ui->regretButton,SIGNAL(clicked(void)),ui->boardui,SLOT(regretBinding(void)));
        QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),ui->boardui,SLOT(giveupBinding(void)));
        gobangboard->setParent(this);
    }
    GobangAI* findAI;
    if((findAI = this->findChild<GobangAI *>(QString(),Qt::FindDirectChildrenOnly)))
    {
        QObject::disconnect(findAI,0,0,0);
        QObject::disconnect(gobangboard,0,findAI,0);
    }
    ui->boardui->newGame(LOCALPVP);
}

void MainWindow::on_actionLocal_PvC_triggered()
{
    GobangBoard *gobangboard;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
        //gobangboard >> board->ui
        QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation);
        QObject::connect(gobangboard,SIGNAL(requestGameover(int)),ui->boardui,SLOT(gameOver(int)));
        // board->ui >> gobangboard
        QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)));
        QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
        QObject::connect(ui->boardui,SIGNAL(requestRegret(int)),gobangboard,SLOT(regret(int)));
        QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)));
        QObject::connect(ui->boardui,SIGNAL(requestLoadBoard(int,int,QVector<int>)),gobangboard,SLOT(loadBoard(int,int,QVector<int>)));
        QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)));
        //gobangboard >> ui->other_widgets
        QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)));
        QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)));
        //ui->buttons >> ui->boardui
        QObject::connect(ui->regretButton,SIGNAL(clicked(void)),ui->boardui,SLOT(regretBinding(void)));
        QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),ui->boardui,SLOT(giveupBinding(void)));
        gobangboard->setParent(this);
    }
    GobangAI* ai=NULL;
    if(!(ai=this->findChild<GobangAI *>(QString(),Qt::FindDirectChildrenOnly)))
    {
        ai = new GobangAI(WHITE);
        ai->setParent(this);
    }

    QObject::disconnect(ai,0,0,0);
    QObject::disconnect(gobangboard,0,ai,0);

    QObject::connect(gobangboard,&GobangBoard::boardChange,ai,&GobangAI::makeDecision);
    QObject::connect(ai,SIGNAL(aiRequestPlay(int,int)),gobangboard,SLOT(play(int,int)));
    ui->boardui->newGame(LOCALPVC);
}

void MainWindow::on_actionLoad_Game_triggered()
{
    this->ui->boardui->loadGame();
}

void MainWindow::on_actionSave_Game_triggered()
{
    this->ui->boardui->save();
}

void MainWindow::on_actionOnline_PvP_triggered()
{

    GobangBoard *gobangboard;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
        //gobangboard >> board->ui
        QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation);
        QObject::connect(gobangboard,SIGNAL(requestGameover(int)),ui->boardui,SLOT(gameOver(int)));
        // board->ui >> gobangboard
        QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
        QObject::connect(ui->boardui,&BoardUi::requestRegret,gobangboard,&GobangBoard::regret);
        QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)));
        QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)));
        QObject::connect(ui->boardui,SIGNAL(requestSetTime(int,int)),gobangboard,SLOT(setTime(int,int)));
        QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)));
        //gobangboard >> ui->other_widgets
        QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)));
        QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)));
        client *cli;
        if(!this->findChild<client *>(QString(),Qt::FindDirectChildrenOnly)){
            cli= new client(this);

            QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),cli,SLOT(sendLocalPlay(int,int)));
            QObject::connect(cli,SIGNAL(getRemotePlay(int,int)),gobangboard,SLOT(play(int,int)));
            QObject::connect(cli,SIGNAL(RemotePlayerReady(int)),ui->boardui,SLOT(newGame(int)));
            QObject::connect(ui->regretButton,SIGNAL(clicked(void)),cli,SLOT(sendRegretRequest()));
            QObject::connect(cli,SIGNAL(excuteRegret(int)),gobangboard,SLOT(regret(int)));
            QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),cli,SLOT(sendGiveupRequest()));
            QObject::connect(cli,SIGNAL(excuteGiveup(int)),gobangboard,SLOT(giveup(int)));
        }
        cli->setServer("localhost",33333);//according to the server's IP and port
    }
    GobangAI* findAI;
    if((findAI = this->findChild<GobangAI *>(QString(),Qt::FindDirectChildrenOnly)))
    {
        QObject::disconnect(findAI,0,0,0);
        QObject::disconnect(gobangboard,0,findAI,0);
    }


}
