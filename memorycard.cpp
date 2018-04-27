#include "memorycard.h"
#include "memorygameboard.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QTimer>
#include <QtWidgets/QGraphicsSceneMouseEvent>


MemoryCard::MemoryCard(QGraphicsScene *scene) :
    QObject(nullptr),
    QGraphicsPixmapItem(nullptr),
    _isFace(false)
{
    scene->addItem(this);
}

MemoryCard::MemoryCard(const QPixmap &face, const QPixmap &back, QGraphicsScene *scene, unsigned id) :
    QObject(nullptr),
    QGraphicsPixmapItem(back, nullptr),
    _face(face),
    _back(back),
    _isFace(false),
    _id(id)
{
    scene->addItem(this);
}

void MemoryCard::flip(const char *slotName)
{
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos", this);
    posAnimation->setStartValue(pos());
    posAnimation->setKeyValueAt(0.5, QPoint(x() + (_back.width() * 0.1), y() + (_back.height() * 0.1)));
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

void MemoryCard::flipToBack()
{
    flip(SLOT(setToBack()));
}

void MemoryCard::flipToFace()
{
    flip(SLOT(setToFace()));
}

void MemoryCard::setToBack()
{
    _isFace = false;
    setPixmap(_back);
}

void MemoryCard::setToFace()
{
    _isFace = true;
    setPixmap(_face);
}

bool MemoryCard::isFace() const
{
    return _isFace;
}

void MemoryCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MemoryGameBoard *board = (MemoryGameBoard*)scene();

    if (!_isFace && board->canReveal())
    {
        flipToFace();

        if (board->lastRevealed())
        {
            MemoryCard *other = board->lastRevealed();
            board->setLastRevealed(0);

            if (this->_id == other->_id)
            {
                QTimer::singleShot(500, this, SLOT(flyOut()));
                QTimer::singleShot(500, other, SLOT(flyOut()));
                QTimer::singleShot(900, board, SLOT(enableReveal()));
            }
            else
            {
                QTimer::singleShot(1000, this, SLOT(flipToBack()));
                QTimer::singleShot(1000, other, SLOT(flipToBack()));
                QTimer::singleShot(1000, board, SLOT(enableReveal()));
            }
        }
        else
        {
            board->setLastRevealed(this);
        }
    }

    event->accept();
}

void MemoryCard::flyOut()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos", this);
    animation->setEndValue(QPoint(- 10 * _back.width(), - 10 * _back.height()));
    animation->setDuration(400);
    animation->setEasingCurve(QEasingCurve::InElastic);
    connect(animation, SIGNAL(finished()), this, SIGNAL(matched()));
    connect(animation, SIGNAL(finished()), this, SLOT(deleteLater()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MemoryCard::saveData(QDataStream &stream) const
{
    stream << _id << _face << _back << _isFace << pos();
}

void MemoryCard::loadData(QDataStream &stream)
{
    QPointF pos;
    stream >> _id >> _face >> _back >> _isFace >> pos;
    setPos(pos);
    setPixmap(_isFace ? _face : _back);
}
