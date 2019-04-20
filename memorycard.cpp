#include "memorycard.h"
#include "memorygameboard.h"

#include <iostream>


MemoryCard::MemoryCard(MemoryGameBoard *scene) :
    QObject(nullptr),
    QGraphicsPixmapItem(nullptr),
    m_isFace(false)
{
    scene->addItem(this);
}

MemoryCard::MemoryCard(const QPixmap &face, const QPixmap &back, MemoryGameBoard *scene, int id) :
    QObject(nullptr),
    QGraphicsPixmapItem(back, nullptr),
    m_face(face),
    m_back(back),
    m_isFace(false),
    m_id(id)
{
    scene->addItem(this);
}

void MemoryCard::flip(const char *slotName)
{
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos", this);
    posAnimation->setStartValue(pos());
    posAnimation->setKeyValueAt(0.5, QPointF(x() + (m_back.width() * 0.1), y() + (m_back.height() * 0.1)));
    posAnimation->setEndValue(pos());
    posAnimation->setDuration(200);
    posAnimation->setEasingCurve(QEasingCurve::OutInExpo);

    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "scale", this);
    scaleAnimation->setStartValue(1);
    scaleAnimation->setKeyValueAt(0.5, 0.8);
    scaleAnimation->setEndValue(1);
    scaleAnimation->setDuration(200);
    scaleAnimation->setEasingCurve(QEasingCurve::OutInExpo);

    group->addAnimation(posAnimation);
    group->addAnimation(scaleAnimation);
    QTimer::singleShot(100, this, slotName);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MemoryCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MemoryGameBoard *board = static_cast<MemoryGameBoard*>(scene());

    // MemoryCard is not facing up and we can reveal other cards
    if (!m_isFace && board->canReveal())
    {
        // Flip the clicked on MemoryCard
        flipToFace();

        // Board has another MemoryCard facing up
        if (board->getLastRevealedCard())
        {
            MemoryCard *other = board->getLastRevealedCard();
            board->setLastRevealedCard(nullptr);

            // Cards match, make them both disappear and enable revealing of board. All after a certain time slot (in ms)
            if (this->m_id == other->m_id)
            {
                QTimer::singleShot(500, this, SLOT(disappear()));
                QTimer::singleShot(500, other, SLOT(disappear()));
                QTimer::singleShot(900, board, SLOT(enableReveal()));
            }
            // Cards didn't match, flip them face down, enable revealing of board, change the player. All with a larger time slot (in ms).
            else
            {
                QTimer::singleShot(1000, this, SLOT(flipToBack()));
                QTimer::singleShot(1000, other, SLOT(flipToBack()));
                QTimer::singleShot(1000, board, SLOT(enableReveal()));
                QTimer::singleShot(1000, board, SLOT(playerChange()));
            }
        }
        // Flipped MemoryCard is the only MemoryCard facing up.
        else
            board->setLastRevealedCard(this);
    }

    event->accept();
}

bool MemoryCard::isFace() const
{
    return m_isFace;
}

void MemoryCard::saveData(QDataStream &stream) const
{
    stream << m_id << m_face << m_back << m_isFace << pos();
}

void MemoryCard::loadData(QDataStream &stream)
{
    QPointF pos;
    stream >> m_id >> m_face >> m_back >> m_isFace >> pos;
    setPos(pos);
    setPixmap(m_isFace ? m_face : m_back);
}

void MemoryCard::flipToBack()
{
    flip(SLOT(setToBack()));
}

void MemoryCard::flipToFace()
{
    flip(SLOT(setToFace()));
}

void MemoryCard::setToFace()
{
    m_isFace = true;
    setPixmap(m_face);
}

void MemoryCard::setToBack()
{
    m_isFace = false;
    setPixmap(m_back);
}

void MemoryCard::disappear()
{
    this->matched();
    this->deleteLater();
}
