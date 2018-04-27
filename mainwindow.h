#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    unsigned m_elapsedSteps;
    Ui::MainWindow *ui;

protected:
    virtual void showEvent(QShowEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onGameWon();
    void startGame();
    void loadGame();
    void saveGame();
    void surrender();
    void onElapsedStepsChanged(unsigned n);
};

#endif // MAINWINDOW_H
