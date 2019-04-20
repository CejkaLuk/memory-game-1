#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QCloseEvent>
#include <QStatusBar>
#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_playedSteps(0),
    m_justSurrendered(false),
    ui(new Ui::MainWindow)
{
    // Set a fixed size to suit all systems
    this->setFixedSize(QSize(1280, 720));

    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    ui->graphicsView->setScene(scene);

    // Menus on top left and their icons + shortucts
    QIcon iconNewGame = QIcon(":icons/newGame.png");
    actNewGame = new QAction(tr("&New Game"), this);
    actNewGame->setShortcut(tr("CTRL+R"));
    actNewGame->setIcon(iconNewGame);
    actNewGame->setToolTip(tr("Start a new game"));
    actNewGame->setStatusTip(tr("Start a new game"));

    QIcon iconSurrender = QIcon(":icons/surrender.png");
    actSurrender = new QAction(tr("Surrender"), this);
    actSurrender->setShortcut(tr("ESC"));
    actSurrender->setIcon(iconSurrender);
    actSurrender->setToolTip(tr("Surrender the current game"));
    actSurrender->setStatusTip(tr("Surrender the current game"));

    QIcon iconSave = QIcon(":icons/saveGame.png");
    actSave = new QAction(tr("&Save"), this);
    actSave->setShortcut(tr("CTRL+S"));
    actSave->setIcon(iconSave);
    actSave->setToolTip(tr("Save"));
    actSave->setStatusTip(tr("Save the current state of the game"));

    QIcon iconLoad = QIcon(":icons/loadGame.png");
    actLoad = new QAction(tr("&Load"), this);
    actLoad->setShortcut(tr("CTRL+O"));
    actLoad->setIcon(iconLoad);
    actLoad->setToolTip(tr("Load"));
    actLoad->setStatusTip(tr("Load a previously saved state of the game"));

    QIcon iconQuit = QIcon(":icons/quit.png");
    actQuit = new QAction(tr("&Quit"), this);
    actQuit->setShortcut(tr("CTRL+Q"));
    actQuit->setIcon(iconQuit);
    actQuit->setToolTip(tr("Exit the program"));
    actQuit->setStatusTip(tr("Exit the program"));

    QIcon iconAbout = QIcon(":icons/about.png");
    actAbout = new QAction(tr("&About"), this);
    actAbout->setShortcut(tr("CTRL+Shift+A"));
    actAbout->setIcon(iconAbout);
    actAbout->setToolTip(tr("About the Author and program"));
    actAbout->setStatusTip(tr("About the Author and program"));

    mnuFile = new QMenu(tr("&File"), this);
    mnuFile->addAction(actNewGame);
    mnuFile->addAction(actSurrender);
    mnuFile->addSeparator();
    mnuFile->addAction(actSave);
    mnuFile->addAction(actLoad);
    mnuFile->addSeparator();
    mnuFile->addAction(actQuit);

    mnuHelp = new QMenu(tr("&Help"), this);
    mnuHelp->addAction(actAbout);

    this->menuBar()->addMenu(mnuFile);
    this->menuBar()->addMenu(mnuHelp);

    // Connect all the buttons to their respective function
    connect(actNewGame, SIGNAL(triggered()), this, SLOT(startGame()));
    connect(actSurrender, SIGNAL(triggered()), this, SLOT(surrender()));
    connect(actSave, SIGNAL(triggered()), this, SLOT(saveGame()));
    connect(actLoad, SIGNAL(triggered()), this, SLOT(loadGame()));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    statusBar()->showMessage(tr("Ready"));
    setWindowTitle(tr("Memory Game"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, tr("Really quit?"),
                              tr("Do you wish to quit?")) == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::showAbout()
{
    // This is too complicated for what it could be, but it looks much better than just a QMessageBox
    QDialog *aboutDialog = new QDialog;
    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *vBox = new QVBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *lMain = new QVBoxLayout;
    QLabel *aboutText = new QLabel(tr("Memory game made as part of the "
                                      "\nassessment for the subject POGR1 at CTU FNSPE."
                                      "\n"
                                      "\nWinter semester, school year 2018/2019"
                                      "\n"
                                      "\nAuthor: Lukáš Čejka"
                                      "\n"
                                      "\nContact: lukas.ostatek@gmail.com"));
    QPushButton *okButton = new QPushButton(tr("Ok"));

    aboutDialog->setWindowTitle(tr("About"));

    // Add text to the vertical box layout
    vBox->addWidget(aboutText);

    // Set the layout of the group box (nicer visuals than QMessageBox) to be the vertical box
    groupBox->setLayout(vBox);

    // Create a button layout
    btnLayout->addStretch();
    btnLayout->addWidget(okButton);

    // Add the groub box and buttons to the main layout
    lMain->addWidget(groupBox);
    lMain->addLayout(btnLayout);

    // Set the layout of the dialog to be the main layout
    aboutDialog->setLayout(lMain);

    connect(okButton, SIGNAL(clicked()), aboutDialog, SLOT(close()));
    aboutDialog->exec();
}

void MainWindow::onGameWon()
{
    bool playerOneVictorious = true;

    // Player 1 has less cards matched than player 2
    if (m_playerOneMatchedCardCount < m_playerTwoMatchedCardCount)
        playerOneVictorious = false;

    // Both players have the same amount of matched cards
    if (m_playerOneMatchedCardCount == m_playerTwoMatchedCardCount)
        QMessageBox::information(this, tr("Game Over!"), tr("It's a draw!\n\n"
                                                            "Player 1 achieved a score of %1\n"
                                                            "Player 2 achieved a score of %2").arg(m_playerOneMatchedCardCount).arg(m_playerTwoMatchedCardCount), QMessageBox::Ok);
    // Player one is victorious
    else if (playerOneVictorious)
        QMessageBox::information(this, tr("Game Over!"), tr("Congratulations on your victory Player 1! \n\n"
                                                            "You have beaten Player 2 with a score of %1 compared to %2.").arg(m_playerOneMatchedCardCount).arg(m_playerTwoMatchedCardCount), QMessageBox::Ok);
    // Player two is victorious
    else
        QMessageBox::information(this, tr("Game Over!"), tr("Congratulations on your victory Player 2! \n\n"
                                                            "You have beaten Player 1 with a score of %2 compared to %1.").arg(m_playerOneMatchedCardCount).arg(m_playerTwoMatchedCardCount), QMessageBox::Ok);

    m_playedSteps = 0;
    startGame();
}

// Function to avoid repeating code in startGame()
void MainWindow::setupGame(MemoryGameBoard *board)
{
    if (ui->graphicsView->scene())
        ui->graphicsView->scene()->deleteLater();

    // Set the scene of graphicsView to be the game board
    ui->graphicsView->setScene(board);

    // Connect all variables and their possible changes
    connect(board, &MemoryGameBoard::gameWon, this, &MainWindow::onGameWon);
    connect(board, &MemoryGameBoard::playedStepsChanged, this, &MainWindow::onPlayedStepsChanged);
    connect(board, &MemoryGameBoard::activePlayerChanged, this, &MainWindow::onActivePlayerChanged);
    connect(board, &MemoryGameBoard::playerMatchedCardCountChanged, this, &MainWindow::onPlayerMatchedCardCountChanged);

    // Set player information
    m_playerOneActive = true;
    m_playerOneMatchedCardCount = 0;
    m_playerTwoMatchedCardCount = 0;

    // Set visuals of the player boxes and text
    // Always starts out with Player 1 playing first
    // Active player's text font is bold and the background of the QLabel box is green
    QFont font = ui->playerOne->font();
    font.setBold(true);
    ui->playerOne->setFont(font);
    ui->playerOne->setStyleSheet("QLabel { background-color : green; }");
    ui->playerOneCardsMatchedCount->setText(QString::number(m_playerOneMatchedCardCount));

    // Non-Active player's text font is normal and the background of the QLabel box is white
    ui->playerTwo->setStyleSheet("QLabel { background-color : white; }");
    ui->playerTwoCardsMatchedCount->setText(QString::number(m_playerTwoMatchedCardCount));

    // Set the background color of the game board, set the size and start the game.
    board->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
    board->setSceneRect(ui->graphicsView->rect());
    board->startGame();
}

void MainWindow::startGame()
{
    MemoryGameBoard *board = new MemoryGameBoard(this, ui->rowBox->value(), ui->columnBox->value());

    // We have just surrendered, we don't want a 'abandon current progress' notification
    if (m_justSurrendered)
    {
        setupGame(board);
        m_justSurrendered = false;
    }
    else
    {
        // The player has revealed at least two cards (elapsedSteps > 0)
        if (m_playedSteps > 0 && QMessageBox::No == QMessageBox::question(this,
                                                                           tr("New game?"),
                                                                           tr("You will lose your progress if you start a new game.\n\n"
                                                                              "Do you wish to start a new game?"),
                                                                           QMessageBox::Yes, QMessageBox::No))
            return;

        setupGame(board);
        m_justSurrendered = false;
    }
}

void MainWindow::surrender()
{
    // Disable usage if the game was just surrendered to avoid crashing repeated use
    if (m_justSurrendered)
        return;

    MemoryGameBoard *board = static_cast<MemoryGameBoard*>(ui->graphicsView->scene());

    // Some steps have been played, since the players would lose knowledge of already revealed card positions, we don't care about matched card count
    if (m_playedSteps > 0)
    {
        // Player clicked no to surrendering, do nothing and return
        if (QMessageBox::No == QMessageBox::question(this,
                                                      tr("Surrender?"),
                                                      tr("You will lose your progress if you surrender.\n\n"
                                                         "Are you sure you want to surrender?"),
                                                      QMessageBox::Yes, QMessageBox::No))
            return;

        board->surrenderGame();
        m_justSurrendered = true;
    }
    // Surrender the game, without asking
    else
    {
        board->surrenderGame();
        m_justSurrendered = true;
    }
}

void MainWindow::onPlayedStepsChanged(unsigned n)
{
    m_playedSteps = n;
    // Show played steps on the ui QLabel
    QString text = tr("Steps so far: %1").arg(QString::number(n));
    ui->stepsLabel->setText(text);
}

void MainWindow::onActivePlayerChanged(bool playerOne)
{
     m_playerOneActive = playerOne;
     // Plater 1 has just been changed to active, set his box's font and style to bold and green (set Player 2's to normal and white)
     if(m_playerOneActive)
     {
         ui->playerOne->setStyleSheet("QLabel { background-color: green; font: bold; }");
         ui->playerTwo->setStyleSheet("QLabel { background-color: white; font: normal; }");
     }
     // Other way around
     else
     {
         ui->playerTwo->setStyleSheet("QLabel { background-color: green; font: bold; }");
         ui->playerOne->setStyleSheet("QLabel { background-color: white; font: normal; }");
     }

}

 void MainWindow::onPlayerMatchedCardCountChanged(unsigned n)
 {
     // Player 1 is active and therefore made the match, save the new value and display it
     if (m_playerOneActive)
     {
         m_playerOneMatchedCardCount = n;
         ui->playerOneCardsMatchedCount->setText(QString::number(m_playerOneMatchedCardCount));
     }
     // Player 2 is active and therefore made the match, save the new value and display it
     else
     {
         m_playerTwoMatchedCardCount = n;
         ui->playerTwoCardsMatchedCount->setText(QString::number(m_playerTwoMatchedCardCount));
     }
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

         MemoryGameBoard *board = static_cast<MemoryGameBoard*>(ui->graphicsView->scene());
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

        // Connect all the variables, for possible changes
        connect(board, &MemoryGameBoard::gameWon, this, &MainWindow::onGameWon);
        connect(board, &MemoryGameBoard::playedStepsChanged, this, &MainWindow::onPlayedStepsChanged);
        connect(board, &MemoryGameBoard::activePlayerChanged, this, &MainWindow::onActivePlayerChanged);
        connect(board, &MemoryGameBoard::playerMatchedCardCountChanged, this, &MainWindow::onPlayerMatchedCardCountChanged);

        board->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
        board->setSceneRect(ui->graphicsView->rect());

        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QDataStream stream(&f);
        board->loadData(stream);

        // Once all the data is loaded from the stream, we can load it into the application.
        // Setup the loaded values
        onPlayedStepsChanged(board->getPlayedSteps());
        onActivePlayerChanged(board->getPlayerOneActive());

        // PlayerMatchedCount has to be loaded manually, because we only have one function for both, we can't input their scores individually.
        m_playerOneMatchedCardCount = board->getPlayerOneMatchedCardCount();
        m_playerTwoMatchedCardCount = board->getPlayerTwoMatchedCardCount();

        // Display the playerMatchedCount, because normally onPlayerMatchedCountChanged() would take care of it, but we can't use it here.
        ui->playerOneCardsMatchedCount->setText(QString::number(m_playerOneMatchedCardCount));
        ui->playerTwoCardsMatchedCount->setText(QString::number(m_playerTwoMatchedCardCount));

        f.close();
    }
}


void MainWindow::showEvent(QShowEvent *event)
{
    startGame();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
