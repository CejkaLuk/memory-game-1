#include "memorygameboard.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QTime>
#include <QtWidgets/QGraphicsDropShadowEffect>

MemoryGameBoard::MemoryGameBoard(QObject *parent) :
    QGraphicsScene(parent),
    _rows(3),
    _columns(9),
    _margin(5),
    _elapsedSteps(0),
    _lastRevealed(0),
    _canReveal(true)
{
}

MemoryCard *MemoryGameBoard::lastRevealed() const
{
    return _lastRevealed;
}

void MemoryGameBoard::setLastRevealed(MemoryCard *card)
{
    if (_canReveal)
    {
        if (card == 0)
        {
            _canReveal = false;
            emit elapsedStepsChanged(++_elapsedSteps);
        }
        _lastRevealed = card;
    }
}

bool MemoryGameBoard::canReveal() const
{
    return _canReveal;
}

void MemoryGameBoard::enableReveal()
{
    _canReveal = true;
}

QPixmap MemoryGameBoard::paintCard(char c, QColor bg, QColor fg, unsigned width, unsigned height)
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
    qreal w = (qreal) (sceneRect().width() - 2 * _margin) / (qreal)_columns;
    qreal h = (qreal) (sceneRect().height() - 2 * _margin) / (qreal)_rows;

    QFont font;
    font.setPixelSize(h - 30);

    QPixmap backPixmap = paintCard('?', QColor(159, 206, 0, 255), QColor(255, 255, 255, 255), w - _margin, h - _margin);

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QList<char> chars = generateChars(_rows * _columns);
    bool skipLast = (_rows * _columns) % 2;

    qsrand(QTime::currentTime().msec());

    for (unsigned i = 0; i < _rows; i++)
    {
        for (unsigned j = 0; j < _columns; j++)
        {
            if (skipLast && j == _columns - 1 && i == _rows - 1)
            {
                continue;
            }

            char ch = chars[qrand() % chars.count()];
            chars.removeOne(ch);

            MemoryCard *card = new MemoryCard(paintCard(ch, QColor(230, 230, 230, 255), QColor(20, 20, 20, 255), w - _margin, h - _margin),
                                              backPixmap,
                                              this,
                                              ch);
            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(card);
            effect->setColor(QColor(0, 0, 0, 200));
            effect->setOffset(0);
            effect->setBlurRadius(_margin * 1.2);
            card->setGraphicsEffect(effect);

            items.append(card);
            connect(card, SIGNAL(matched()), this, SLOT(cardMatched()));
            card->show();

            QPropertyAnimation *animation = new QPropertyAnimation(card, "pos", this);
            animation->setStartValue(QPointF(j * w + _margin, -h));
            animation->setEndValue(QPointF(j * w + _margin, i * h + _margin));
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
    foreach (MemoryCard *card, items)
        card->flipToFace();
    _canReveal = false;
    _lastRevealed = 0;
}

void MemoryGameBoard::cardMatched()
{
    items.removeOne((MemoryCard*)sender());
    if (items.count() == 0)
        emit gameWon();
}

QList<char> MemoryGameBoard::generateChars(unsigned n)
{
    QList<char> chars;
    if (n % 2)
        n--;
    unsigned i = 0;
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
    stream << _rows << _columns << _margin << _elapsedSteps << _canReveal << items.count() << items.indexOf(_lastRevealed);
    foreach(MemoryCard *card, items)
        card->saveData(stream);
}

void MemoryGameBoard::loadData(QDataStream &stream)
{
    int cardCount, lastRevealedIndex;
    stream >> _rows >> _columns >> _margin >> _elapsedSteps >> _canReveal >> cardCount >> lastRevealedIndex;
    for (int i = 0; i < cardCount; i++)
    {
        MemoryCard *card = new MemoryCard(this);

        card->loadData(stream);
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(card);
        effect->setColor(QColor(0, 0, 0, 200));
        effect->setOffset(0);
        effect->setBlurRadius(_margin * 1.2);
        card->setGraphicsEffect(effect);

        items.append(card);
        connect(card, SIGNAL(matched()), this, SLOT(cardMatched()));
        card->show();

        if (i == lastRevealedIndex)
            _lastRevealed = card;
    }
}
