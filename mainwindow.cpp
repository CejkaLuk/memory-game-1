#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorygameboard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _elapsedSteps(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    startGame();
    event->accept();
}

void MainWindow::startGame()
{
    if (_elapsedSteps && QMessageBox::No == QMessageBox::question(this, "Sure?", "Do you wish to abandon the current game?", QMessageBox::Yes, QMessageBox::No))
        return;

    if (ui->graphicsView->scene())
        ui->graphicsView->scene()->deleteLater();

    MemoryGameBoard *board = new MemoryGameBoard(this);
    ui->graphicsView->setScene(board);
    connect(board, SIGNAL(gameWon()), this, SLOT(onGameWon()));
    connect(board, SIGNAL(elapsedStepsChanged(uint)), this, SLOT(onElapsedStepsChanged(uint)));
    board->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    board->setSceneRect(ui->graphicsView->rect());
    board->startGame();
}

void MainWindow::onGameWon()
{
    QMessageBox::information(this, "You rock!", "Congratulations, you have won!", QMessageBox::Ok);
    _elapsedSteps = 0;
    startGame();
}

void MainWindow::saveGame()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
    {
        if (QFile::exists(fileName))
            QFile::remove(fileName);

        QFile f(fileName);
        f.open(QIODevice::WriteOnly);
        QDataStream stream(&f);

        MemoryGameBoard *board = (MemoryGameBoard*)ui->graphicsView->scene();
        board->saveData(stream);

        f.close();
    }
}

void MainWindow::loadGame()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        if (ui->graphicsView->scene())
            ui->graphicsView->scene()->deleteLater();

        MemoryGameBoard *board = new MemoryGameBoard(this);
        ui->graphicsView->setScene(board);
        connect(board, SIGNAL(gameWon()), this, SLOT(onGameWon()));
        connect(board, SIGNAL(elapsedStepsChanged(uint)), this, SLOT(onElapsedStepsChanged(uint)));
        board->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
        board->setSceneRect(ui->graphicsView->rect());

        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QDataStream stream(&f);
        board->loadData(stream);
        f.close();
    }
}

void MainWindow::surrender()
{
    MemoryGameBoard *board = (MemoryGameBoard*)ui->graphicsView->scene();
    board->surrenderGame();
}

void MainWindow::onElapsedStepsChanged(unsigned n)
{
    _elapsedSteps = n;
    ui->stepsLabel->setText("Elapsed steps: " + QString::number(n));
}
