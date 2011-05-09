#ifndef MEMORYCARD_H
#define MEMORYCARD_H

#include <QtGui>

class MemoryCard : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    QPixmap _face, _back;
    bool _isFace;
    unsigned _id;

    void flip(const char *slotName);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    explicit MemoryCard(const QPixmap &face, const QPixmap &back, QGraphicsScene *scene, unsigned id);
    bool isFace();

signals:
    void matched();

private slots:
    void setToFace();
    void setToBack();

public slots:
    void flyOut();
    void flipToFace();
    void flipToBack();

};

#endif // MEMORYCARD_H
