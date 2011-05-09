#ifndef MEMORYGAMEBOARD_H
#define MEMORYGAMEBOARD_H

#include <QtGui>
#include "memorycard.h"

class MemoryGameBoard : public QGraphicsScene
{
    Q_OBJECT
    unsigned _rows, _columns, _elapsedSteps;
    MemoryCard *_lastRevealed;
    bool _canReveal;
    QList<MemoryCard*> items;

public:
    explicit MemoryGameBoard(QObject *parent = 0);
    MemoryCard *lastRevealed();
    void setLastRevealed(MemoryCard *card);
    bool canReveal();
    static QPixmap paintCard(char c, QColor bg, QColor fg, unsigned width, unsigned height);

    static QList<char> generateChars(unsigned n);

signals:
    void elapsedStepsChanged(unsigned n);
    void gameWon();

public slots:
    void cardMatched();
    void startGame();
    void surrenderGame();
    void enableReveal();

};

#endif // MEMORYGAMEBOARD_H
