#include "memorygameboard.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QTime>
#include <QtWidgets/QGraphicsDropShadowEffect>

MemoryGameBoard::MemoryGameBoard(QObject *parent) :
    QGraphicsScene(parent),
    m_rows(3),
    m_columns(9),
    m_margin(5),
    m_elapsedSteps(0),
    m_lastRevealed(nullptr),
    m_canReveal(true)
{
}

MemoryCard *MemoryGameBoard::lastRevealed() const
{
    return m_lastRevealed;
}

void MemoryGameBoard::setLastRevealed(MemoryCard *card)
{
    if (m_canReveal)
    {
        if (!card)
        {
            m_canReveal = false;
            emit elapsedStepsChanged(++m_elapsedSteps);
        }
        m_lastRevealed = card;
    }
}

bool MemoryGameBoard::canReveal() const
{
    return m_canReveal;
}

void MemoryGameBoard::enableReveal()
{
    m_canReveal = true;
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

void MemoryGameBoard::startGame()
{
    qreal ww = static_cast<qreal>(sceneRect().width() - 2 * m_margin) / static_cast<qreal>(m_columns);
    qreal hh = static_cast<qreal>(sceneRect().height() - 2 * m_margin) / static_cast<qreal>(m_rows);
    int h = static_cast<int>(hh);
    int w = static_cast<int>(ww);

    QFont font;
    font.setPixelSize(h - 30);

    QPixmap backPixmap = paintCard('?', QColor(159, 206, 0, 255), QColor(255, 255, 255, 255), w - m_margin, h - m_margin);

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QList<char> chars = generateChars(m_rows * m_columns);
    bool skipLast = (m_rows * m_columns) % 2;

    qsrand(static_cast<quint32>(QTime::currentTime().msec()));

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            if (skipLast && j == m_columns - 1 && i == m_rows - 1)
            {
                continue;
            }

            char ch = chars[qrand() % chars.count()];
            chars.removeOne(ch);

            MemoryCard *card = new MemoryCard(paintCard(ch, QColor(230, 230, 230, 255), QColor(20, 20, 20, 255), w - m_margin, h - m_margin),
                                              backPixmap,
                                              this,
                                              ch);
            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(card);
            effect->setColor(QColor(0, 0, 0, 200));
            effect->setOffset(0);
            effect->setBlurRadius(m_margin * 1.2);
            card->setGraphicsEffect(effect);

            m_items.append(card);
            connect(card, &MemoryCard::matched, this, &MemoryGameBoard::cardMatched);
            card->show();

            QPropertyAnimation *animation = new QPropertyAnimation(card, "pos", this);
            animation->setStartValue(QPointF(j * w + m_margin, -h));
            animation->setEndValue(QPointF(j * w + m_margin, i * h + m_margin));
            animation->setDuration(1000);
            animation->setEasingCurve(QEasingCurve::OutBounce);
            group->addAnimation(animation);
        }
    }

    emit elapsedStepsChanged(0);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MemoryGameBoard::surrenderGame()
{
    foreach (MemoryCard *card, m_items)
        card->flipToFace();
    m_canReveal = false;
    m_lastRevealed = nullptr;
}

void MemoryGameBoard::cardMatched()
{
    auto *card = qobject_cast<MemoryCard*>(sender());
    m_items.removeOne(card);
    if (m_items.count() == 0)
        emit gameWon();
}

QList<char> MemoryGameBoard::generateChars(int n)
{
    QList<char> chars;
    if (n % 2)
        n--;
    int i = 0;
    for (char c = '0'; i < n && c <= '9'; c+=1, i+=2)
    {
        chars.push_back(c);
        chars.push_back(c);
    }
    for (char c = 'A'; i < n && c <= 'Z'; c+=1, i+=2)
    {
        chars.push_back(c);
        chars.push_back(c);
    }
    return chars;
}

void MemoryGameBoard::saveData(QDataStream &stream) const
{
    stream << m_rows << m_columns << m_margin << m_elapsedSteps << m_canReveal << m_items.count() << m_items.indexOf(m_lastRevealed);
    foreach(MemoryCard *card, m_items)
        card->saveData(stream);
}

void MemoryGameBoard::loadData(QDataStream &stream)
{
    int cardCount, lastRevealedIndex;
    stream >> m_rows >> m_columns >> m_margin >> m_elapsedSteps >> m_canReveal >> cardCount >> lastRevealedIndex;
    for (int i = 0; i < cardCount; i++)
    {
        MemoryCard *card = new MemoryCard(this);

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
            m_lastRevealed = card;
    }
}
