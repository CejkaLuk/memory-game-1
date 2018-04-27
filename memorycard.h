#ifndef MEMORYCARD_H
#define MEMORYCARD_H

#include <QtCore/QObject>
#include <QtWidgets/QGraphicsPixmapItem>

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
    explicit MemoryCard(QGraphicsScene *scene);
    explicit MemoryCard(const QPixmap &face, const QPixmap &back, QGraphicsScene *scene, unsigned id);
    bool isFace() const;
    void saveData(QDataStream &stream) const;
    void loadData(QDataStream &stream);

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
