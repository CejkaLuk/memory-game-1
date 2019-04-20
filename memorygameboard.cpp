#include "memorygameboard.h"

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QGraphicsView>
#include <QSettings>

#include <iostream>


MemoryGameBoard::MemoryGameBoard(QObject *parent, int rows, int columns) :
    QGraphicsScene(parent),
    m_rows(rows),
    m_columns(columns),
    m_margin(5), // Gap between cards
    m_playedSteps(0),
    m_lastRevealedCard(nullptr),
    m_canReveal(true),
    m_playerOneActive(true),
    m_playerOneMatchedCardCount(0),
    m_playerTwoMatchedCardCount(0)
{
}

MemoryCard *MemoryGameBoard::getLastRevealedCard() const
{
    return m_lastRevealedCard;
}

void MemoryGameBoard::setLastRevealedCard(MemoryCard *MemoryCard)
{
    // Board can be revealed
    if (m_canReveal)
    {
        // MemoryCard is empty (nullptr)
        if (!MemoryCard)
        {
            m_canReveal = false;
            emit playedStepsChanged(++m_playedSteps);
        }
        // MemoryCard is a normal game MemoryCard
        m_lastRevealedCard = MemoryCard;
    }
}

bool MemoryGameBoard::canReveal() const
{
    return m_canReveal;
}

void MemoryGameBoard::saveData(QDataStream &stream) const
{
    stream << m_rows << m_columns << m_margin << m_playedSteps << m_canReveal << m_playerOneActive << m_playerOneMatchedCardCount << m_playerTwoMatchedCardCount << m_items.count() << m_items.indexOf(m_lastRevealedCard);
    foreach(MemoryCard *MemoryCard, m_items)
        MemoryCard->saveData(stream);
}

void MemoryGameBoard::loadData(QDataStream &stream)
{
    int cardCount, lastRevealedIndex;
    stream >> m_rows >> m_columns >> m_margin >> m_playedSteps >> m_canReveal >> m_playerOneActive >> m_playerOneMatchedCardCount >> m_playerTwoMatchedCardCount >> cardCount >> lastRevealedIndex;
    for (int i = 0; i < cardCount; i++)
    {
        MemoryCard *card = new class MemoryCard(this);

        card->loadData(stream);
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(card);
        effect->setColor(QColor(0, 0, 0, 200));
        effect->setOffset(0);
        effect->setBlurRadius(m_margin * 1.2);
        card->setGraphicsEffect(effect);

        m_items.append(card);
        connect(card, &MemoryCard::matched, this, &MemoryGameBoard::cardMatched);
        card->show();

        if (i == lastRevealedIndex)
            m_lastRevealedCard = card;
    }
}

QPixmap MemoryGameBoard::paintCard(char c, QColor bg, QColor fg, int width, int height)
{
    QFont font;
    font.setPixelSize(height - 15);
    QPixmap pix(width, height);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setFont(font);
    painter.setPen(bg);
    painter.setBrush(bg);
    painter.drawRoundedRect(0, 0, width, height, 10, 10, Qt::RelativeSize);
    painter.setPen(fg);
    painter.drawText(0, 0, width, height, Qt::AlignCenter, QString(c));
    painter.end();
    return pix;
}

QList<char> MemoryGameBoard::generateChars(int n)
{
    QList<char> chars;
    // Even number of cards needed
    if (n % 2)
        n--;
    int i = 0;
    // Generate numbers first
    for (char c = '0'; i < n && c <= '9'; c+=1, i+=2)
    {
        chars.push_back(c);
        chars.push_back(c);
    }
    // We ran out of unique numbers, generate characters
    for (char c = 'A'; i < n && c <= 'Z'; c+=1, i+=2)
    {
        chars.push_back(c);
        chars.push_back(c);
    }
    return chars;
}

void MemoryGameBoard::cardMatched()
{
    auto *card = qobject_cast<MemoryCard*>(sender());
    m_items.removeOne(card);
    // Player 1 was active when MemoryCard was matched, increase his matched MemoryCard count
    if (m_playerOneActive)
        emit playerMatchedCardCountChanged(++m_playerOneMatchedCardCount);
    // Player 2 was active when MemoryCard was matched, increase his matched MemoryCard count
    else
        emit playerMatchedCardCountChanged(++m_playerTwoMatchedCardCount);

    if (m_items.count() == 0)
        emit gameWon();
}

void MemoryGameBoard::startGame()
{
    // This fixes the error for pixels being off on the right at the start.
    QRectF rect = sceneRect();
    rect.setHeight(630);
    rect.setWidth(1000);
    this->setSceneRect(rect);

    // This divides the MemoryGameBoard area into slots for cards, based on the amount of columns and rows.
    qreal ww = static_cast<qreal>(sceneRect().width() - 2 * m_margin) / static_cast<qreal>(m_columns); // qreal is the typedef for double, unless Qt is configured with -qreal float option
    qreal hh = static_cast<qreal>(sceneRect().height() - 2 * m_margin) / static_cast<qreal>(m_rows);   // static_cast converts ints to floats. //sceneRect() holds the area of the scene visualized by this view. In this case, it is MemoryGameBoard.
    int h = static_cast<int>(hh); // number of pixels, one MemoryCard will take up in the MemoryGameBoard area, height of one MemoryCard
    int w = static_cast<int>(ww); // number of pixels, one MemoryCard will take up in the MemoryGameBoard area, width of one MemoryCard

    QFont font;
    font.setPixelSize(h - 30);

    // Paint the face down state of the MemoryCard with a '?' symbol at a font that scales with the window size (number of cards effect the size of each one, so that they all fit.)
    QPixmap backPixmap = paintCard('?', QColor(159, 206, 0, 255), QColor(255, 255, 255, 255), w - m_margin, h - m_margin);

    // Initiate the animation group
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    // Generate the list of characters
    QList<char> chars = generateChars(m_rows * m_columns);

    // Skip the potentially odd MemoryCard
    bool skipLast = (m_rows * m_columns) % 2;

    // Make the character generation random
    qsrand(static_cast<quint32>(QTime::currentTime().msec()));

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            // We have a potential odd MemoryCard on the last position
            if (skipLast && j == m_columns - 1 && i == m_rows - 1)
            {
                continue;
            }

            char ch = chars[qrand() % chars.count()];
            chars.removeOne(ch);

            // Create the MemoryCard, paint its face up state, a position it
            MemoryCard *card = new MemoryCard(paintCard(ch, QColor(230, 230, 230, 255), QColor(20, 20, 20, 255), w - m_margin, h - m_margin),
                                              backPixmap,
                                              this,
                                              ch);

            // Add the MemoryCard to the end of the list
            m_items.append(card);
            connect(card, &MemoryCard::matched, this, &MemoryGameBoard::cardMatched);
            card->show();

            // Create animation for each MemoryCard, to slide in from the top,
            QPropertyAnimation *animation = new QPropertyAnimation(card, "pos", this);
            animation->setStartValue(QPointF(j * w + m_margin, -h));
            animation->setEndValue(QPointF(j * w + m_margin, i * h + m_margin));
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::BezierSpline);
            group->addAnimation(animation);

        }
    }

    emit playedStepsChanged(0);

    // Start the animation of all cards simultaneously and delete after
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MemoryGameBoard::surrenderGame()
{
    // Flip each MemoryCard to face up
    foreach (MemoryCard *MemoryCard, m_items)
        MemoryCard->flipToFace();
    m_canReveal = false;
    m_lastRevealedCard = nullptr;
}

void MemoryGameBoard::enableReveal()
{
    m_canReveal = true;
}

void MemoryGameBoard::playerChange()
{
    // This is here and not in mousePressEvent, so that it is triggered with the singleshot delay.
    // Player 1 is active, change state to false (== Player Two active)
    if (m_playerOneActive)
        setPlayerOneActive(false);
    else
        setPlayerOneActive(true);
}
