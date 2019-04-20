#ifndef MEMORYGAMEBOARD_H
#define MEMORYGAMEBOARD_H

#include <QtWidgets/QGraphicsScene>
#include "memorycard.h"

class MemoryGameBoard : public QGraphicsScene
{
    Q_OBJECT
    int m_rows, m_columns, m_margin; // Number of rows, columns. Size of the gap between cards
    unsigned m_playedSteps; // Steps taken, including mathched non-matched, not dependent on players
    MemoryCard *m_lastRevealedCard; // Last revealed card
    bool m_canReveal; // Enabling reveal of cards in board
    bool m_playerOneActive; // Current active player (only 2 players, keeping track of one is enough)
    unsigned m_playerOneMatchedCardCount, m_playerTwoMatchedCardCount; // Number of matched cards (not pairs of cards) each player has.
    QList<MemoryCard*> m_items; // List of cards

public:
    explicit MemoryGameBoard(QObject *parent = nullptr, int rows = 2, int columns = 4);
    unsigned getPlayedSteps() { return m_playedSteps; }
    MemoryCard *getLastRevealedCard() const;
    void setLastRevealedCard(MemoryCard *card);
    bool canReveal() const;
    bool getPlayerOneActive() { return m_playerOneActive; }
    void setPlayerOneActive(bool playerOne) { emit activePlayerChanged(playerOne); m_playerOneActive = playerOne; }
    unsigned getPlayerOneMatchedCardCount() { return m_playerOneMatchedCardCount; }
    unsigned getPlayerTwoMatchedCardCount() { return m_playerTwoMatchedCardCount; }
    void saveData(QDataStream &stream) const;
    void loadData(QDataStream &stream);

    static QPixmap paintCard(char c, QColor bg, QColor fg, int width, int height); // Create visual aspect of card
    static QList<char> generateChars(int n);

public slots:
    void cardMatched();
    void startGame();
    void surrenderGame();
    void enableReveal();
    void playerChange();

signals:
    void playedStepsChanged(unsigned n);
    void activePlayerChanged(bool playerOne);
    void playerMatchedCardCountChanged(unsigned n);
    void gameWon();

};

#endif // MEMORYGAMEBOARD_H
