#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionQuit_Application_triggered();

    void on_actionLocal_PvP_triggered();

    void on_actionLoad_Game_triggered();

    void on_actionSave_Game_triggered();

    void on_actionOnline_PvP_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
