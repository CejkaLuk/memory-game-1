#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorygameboard.h"

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

void MainWindow::startGame()
{
    if (ui->graphicsView->scene())
        ui->graphicsView->scene()->deleteLater();

    MemoryGameBoard *board = new MemoryGameBoard(this);
    ui->graphicsView->setScene(board);
    connect(board, SIGNAL(gameWon()), this, SLOT(onGameWon()));
    connect(board, SIGNAL(elapsedStepsChanged(uint)), this, SLOT(onElapsedStepsChanged(uint)));
    board->setSceneRect(ui->graphicsView->rect());
    board->startGame();
}

void MainWindow::onGameWon()
{
    QMessageBox::information(this, "You rock!", "Congratulations, you have won!", QMessageBox::Ok);
}

void MainWindow::saveGame()
{

}

void MainWindow::loadGame()
{

}

void MainWindow::onElapsedStepsChanged(unsigned n)
{
    ui->stepsLabel->setText("Elapsed steps: " + QString::number(n));
}
