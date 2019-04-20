#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "memorygameboard.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    unsigned m_playedSteps; // Number of steps played
    bool m_justSurrendered; // If the game had just been surrendered, we don't want messages to ask if we want to start a new game (abandon current)
    bool m_playerOneActive; // Player 1 active status, and player 2 (when Player 1 is false)
    unsigned m_playerOneMatchedCardCount, m_playerTwoMatchedCardCount; // Number of matched cards (not pairs) for each player
    Ui::MainWindow *ui;

    QMenu *mnuFile;
        QAction *actNewGame;
        QAction *actSurrender;
        QAction *actSave;
        QAction *actLoad;
        QAction *actQuit;
    QMenu *mnuHelp;
        QAction *actInstructions;
        QAction *actAbout;
    QAction *actPreferences;

    void closeEvent(QCloseEvent *event);
    void setupGame(MemoryGameBoard *board);

private slots:
    void showAbout();

    void onGameWon();
    void startGame();
    void surrender();
    void onPlayedStepsChanged(unsigned n); // For connecting signals with the board's steps changed
    void onActivePlayerChanged(bool playerOne); // For connecting signals with the board's active player changed
    void onPlayerMatchedCardCountChanged(unsigned n); // For connecting signals with the board's matched card count
    void saveGame();
    void loadGame();

protected:
    virtual void showEvent(QShowEvent *event);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
