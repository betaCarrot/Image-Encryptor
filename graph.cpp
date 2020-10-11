#include "graph.h"

/*!
 * \brief construct a derived class for QGraphicsView.
 *
 * This is the constructor of class Graph which is the derived class of QGraphicsView
 *
 * \param parent parent Qwidget
 */

Graph::Graph(QWidget *parent) :
        QGraphicsView(parent) {
    setInteractive(true);
}

/*!
 * \brief Override the resizeEvent of class QGraphicsView.
 *
 * This is an override function since we want the photo in the QGraphicsView to resize when its parent is resizing
 *
 * \param event this is the QResizeEvent defined by QT
 */

void Graph::resizeEvent(QResizeEvent *event) {
//    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    emit sizechange();
    QGraphicsView::resizeEvent(event);
}
