#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>

/*!
 * \brief A derived class of QGraphicsView that supports resizing when dragged by user.
 */
class Graph: public QGraphicsView {
    Q_OBJECT

public:
    Graph(QWidget *parent = nullptr);

protected:
    virtual void resizeEvent(QResizeEvent*) override;signals:
    void sizechange();
    /*!<  Emit this signal when changing the size of Graph window */
};

#endif // GRAPH_H
