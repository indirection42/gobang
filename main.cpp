#include "mainwindow.h"
#include "gobangboard.h"
#include "boardui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    BoardUi ui;
    GobangBoard gobangboard;
    QObject::connect(&gobangboard,&GobangBoard::boardChange,&ui,&BoardUi::updateInformation);
    QObject::connect(&ui,&BoardUi::requestPlay,&gobangboard,&GobangBoard::play);
    QObject::connect(&ui,&BoardUi::requestRegret,&gobangboard,&GobangBoard::regret);
    ui.show();
    w.show();
    return a.exec();
}
