#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gobangboard.h"
#include "boardui.h"
#include "gobangai.h"
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
    GobangBoard *gobangboard=nullptr;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
    }
    //gobangboard >> board->ui
    QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation,Qt::UniqueConnection);
    // board->ui >> gobangboard
    QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestRegret(int)),gobangboard,SLOT(regret(int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)),Qt::UniqueConnection);
    //gobangboard >> ui->other_widgets
    QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)),Qt::UniqueConnection);
    QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)),Qt::UniqueConnection);
    //ui->buttons >> ui->boardui
    QObject::connect(ui->regretButton,SIGNAL(clicked(void)),ui->boardui,SLOT(regretBinding(void)),Qt::UniqueConnection);
    QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),ui->boardui,SLOT(giveupBinding(void)),Qt::UniqueConnection);
    gobangboard->setParent(this);
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

    GobangBoard *gobangboard=nullptr;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
    }
    //gobangboard >> board->ui
    QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation,Qt::UniqueConnection);
    // board->ui >> gobangboard
    QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestRegret(int)),gobangboard,SLOT(regret(int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)),Qt::UniqueConnection);
    //gobangboard >> ui->other_widgets
    QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)),Qt::UniqueConnection);
    QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)),Qt::UniqueConnection);
    //ui->buttons >> ui->boardui
    QObject::connect(ui->regretButton,SIGNAL(clicked(void)),ui->boardui,SLOT(regretBinding(void)),Qt::UniqueConnection);
    QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),ui->boardui,SLOT(giveupBinding(void)),Qt::UniqueConnection);
    gobangboard->setParent(this);
    GobangAI* ai=NULL;
    if(!(ai=this->findChild<GobangAI *>(QString(),Qt::FindDirectChildrenOnly)))
    {
        ai = new GobangAI(WHITE);
        ai->setParent(this);
    }

    QObject::disconnect(ai,0,0,0);
    QObject::disconnect(gobangboard,0,ai,0);

    QObject::connect(gobangboard,&GobangBoard::boardChange,ai,&GobangAI::makeDecision,Qt::UniqueConnection);
    QObject::connect(ai,SIGNAL(aiRequestPlay(int,int)),gobangboard,SLOT(play(int,int)),Qt::UniqueConnection);
    ui->boardui->newGame(LOCALPVC);
}

void MainWindow::on_actionLoad_Game_triggered()
{
    GobangBoard *gobangboard=nullptr;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
    }
    QObject::connect(this,SIGNAL(requestLoadBoard(int,int,QVector<int>)),gobangboard,SLOT(loadBoard(int,int,QVector<int>)),Qt::UniqueConnection);
    QString path = QFileDialog::getOpenFileName(this, tr("Choose your save"), ".", tr("Saving Files(*.save)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        return;
    } else
    {
        QFile file1(path);
        QTextStream out1(&file1);
        file1.open(QFile::ReadOnly);
        if(!file1.isOpen())
        {
            printf( "nope\n");
            return;
        }
        int b, w;
        int gameMode;
        out1 >> gameMode >> b >> w;
        if(gameMode==LOCALPVP){
            on_actionLocal_PvP_triggered();
        }
        else if(gameMode==LOCALPVC){
            on_actionLocal_PvC_triggered();
        }
        else
            return;
        QVector<int> record;
        while(!out1.atEnd())
        {
            qint32 i;
            out1 >> i;
            record.push_back(i);
        }
        emit requestLoadBoard(b, w,record);
        file1.close();
        return;
    }
}

void MainWindow::on_actionSave_Game_triggered()
{
    this->ui->boardui->save();
}

void MainWindow::on_actionOnline_PvP_triggered()
{
    client *cli;
    GobangBoard *gobangboard;
    if(!(gobangboard=this->findChild<GobangBoard *>(QString(),Qt::FindDirectChildrenOnly))){
        gobangboard=new GobangBoard(this);
    }
    //gobangboard >> board->ui
    QObject::connect(gobangboard,&GobangBoard::boardChange,ui->boardui,&BoardUi::updateInformation,Qt::UniqueConnection);
    // board->ui >> gobangboard
    QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,&BoardUi::requestRegret,gobangboard,&GobangBoard::regret,Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestGiveUp(int)),gobangboard,SLOT(giveup(int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(start(void)),gobangboard,SLOT(start(void)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestSetTime(int,int)),gobangboard,SLOT(setTime(int,int)),Qt::UniqueConnection);
    QObject::connect(ui->boardui,SIGNAL(requestSave(int)),gobangboard,SLOT(save(int)),Qt::UniqueConnection);
    //gobangboard >> ui->other_widgets
    QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->blackLCD),SLOT(display(int)),Qt::UniqueConnection);
    QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->whiteLCD),SLOT(display(int)),Qt::UniqueConnection);

    if(!this->findChild<client *>(QString(),Qt::FindDirectChildrenOnly)){
        cli= new client(this);
    }

    QObject::connect(ui->boardui,SIGNAL(requestPlay(int,int)),cli,SLOT(sendLocalPlay(int,int)),Qt::UniqueConnection);
    QObject::connect(cli,SIGNAL(getRemotePlay(int,int)),gobangboard,SLOT(play(int,int)),Qt::UniqueConnection);
    QObject::connect(cli,SIGNAL(RemotePlayerReady(int)),ui->boardui,SLOT(newGame(int)),Qt::UniqueConnection);
    QObject::connect(ui->regretButton,SIGNAL(clicked(void)),cli,SLOT(sendRegretRequest()),Qt::UniqueConnection);
    QObject::connect(cli,SIGNAL(excuteRegret(int)),gobangboard,SLOT(regret(int)),Qt::UniqueConnection);
    QObject::connect(ui->giveupButton,SIGNAL(clicked(void)),cli,SLOT(sendGiveupRequest()),Qt::UniqueConnection);
    QObject::connect(cli,SIGNAL(excuteGiveup(int)),gobangboard,SLOT(giveup(int)),Qt::UniqueConnection);
    GobangAI* findAI;
    if((findAI = this->findChild<GobangAI *>(QString(),Qt::FindDirectChildrenOnly)))
    {
        QObject::disconnect(findAI,0,0,0);
        QObject::disconnect(gobangboard,0,findAI,0);
    }
    if(cli=this->findChild<client *>(QString(),Qt::FindDirectChildrenOnly)){
        cli->setServer("localhost",33333);//according to the server's IP and port
    }
}
