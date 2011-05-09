#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    unsigned _elapsedSteps;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:

private slots:
    void onGameWon();
    void startGame();
    void loadGame();
    void saveGame();
    void onElapsedStepsChanged(unsigned n);
};

#endif // MAINWINDOW_H
