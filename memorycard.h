#ifndef MEMORYCARD_H
#define MEMORYCARD_H

#include <QObject>
#include <QtWidgets>

class MemoryGameBoard;

class MemoryCard : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    QPixmap m_face, m_back; // Card face up, Card face down
    bool m_isFace; // State of the card faces
    int m_id; // ID of the card

    void flip(const char *slotName);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    explicit MemoryCard(MemoryGameBoard *scene);
    explicit MemoryCard(const QPixmap &face, const QPixmap &back, MemoryGameBoard *scene, int id);
    bool isFace() const;
    void saveData(QDataStream &stream) const;
    void loadData(QDataStream &stream);

public slots:
    void disappear(); // To make the card disappear if matched
    void flipToFace(); // Animation to flip the card to face.
    void flipToBack(); // Animation to flip the card to back.

private slots:
    void setToFace(); // Set the card to face up.
    void setToBack(); // Set the card to face down.

signals:
    void matched(); // Signal to trigger the MemoryGameBoard cardMatched function

};

#endif // MEMORYCARD_H
