#include "mainwindow.h"
#include "gobangboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    GobangBoard board;
    board.show();
    w.show();

    return a.exec();
}
