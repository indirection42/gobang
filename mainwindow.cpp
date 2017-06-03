#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gobangboard.h"
#include "boardui.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     BoardUi *boardui=new BoardUi(this);
     GobangBoard *gobangboard=new GobangBoard(this);
     ui->gridLayout->addWidget(boardui);
     QObject::connect(gobangboard,&GobangBoard::boardChange,boardui,&BoardUi::updateInformation);
     QObject::connect(boardui,SIGNAL(requestPlay(int,int)),gobangboard,SLOT(play(int,int)));
     QObject::connect(boardui,&BoardUi::requestRegret,gobangboard,&GobangBoard::regret);
     QObject::connect(gobangboard,SIGNAL(blackTimeChange(int)),(this->ui->lcdNumber),SLOT(display(int)));
     QObject::connect(gobangboard,SIGNAL(whiteTimeChange(int)),(this->ui->lcdNumber_2),SLOT(display(int)));
     gobangboard->startTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

