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
    Ui::MainWindow *ui;

protected:
    virtual void showEvent(QShowEvent *event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onGameWon();
    void startGame();
    void loadGame();
    void saveGame();
    void onElapsedStepsChanged(unsigned n);
};

#endif // MAINWINDOW_H
