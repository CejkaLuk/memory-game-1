#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorygameboard.h"

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5/QtMaemo5>
#endif

#if defined(HAVE_OPENGL)
#include <QtOpenGL>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _elapsedSteps(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    ui->graphicsView->setScene(scene);

#if defined(MOBILE)
    ui->surrenderButton->hide();
    ui->newGameButton->hide();
    ui->saveButton->hide();
    ui->loadButton->hide();
    ui->stepsLabel->hide();

    ui->graphicsView->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    ui->graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
#else
    ui->menuBar->hide();
#endif

#if defined(HAVE_OPENGL)
    ui->graphicsView->setViewport(new QGLWidget(ui->graphicsView));
#endif
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
    if (_elapsedSteps && QMessageBox::No == QMessageBox::question(this, tr("Sure?"), tr("Do you wish to abandon the current game?"), QMessageBox::Yes, QMessageBox::No))
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
#if defined(Q_WS_MAEMO_5)
    QMaemo5InformationBox::information(0, "<b>" + tr("You rock!") + "</b><br />" + tr("Congratulations, you have won!"));
#else
    QMessageBox::information(this, tr("You rock!"), tr("Congratulations, you have won!"), QMessageBox::Ok);
#endif
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
    QString text = tr("Steps so far: %1").arg(QString::number(n));
#if defined(MOBILE)
    setWindowTitle(text);
#else
    ui->stepsLabel->setText(text);
#endif
}
