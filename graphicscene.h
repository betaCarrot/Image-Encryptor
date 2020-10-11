#ifndef GraphicsScene_H
#define GraphicsScene_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QVector>
#include <QPair>
#include <vector>
#include <QString>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsView>
#include "graph.h"

/*!
 * \brief A class used to display a background pixmap and a foreground pixmap based on the image that it contains.
 */
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(Graph* view,QObject *parent = nullptr);
    ~GraphicsScene();
    void setImage(const QImage &img);
    QImage getImage() const;

private:
    static const double BACKGROUND_Z_VALUE; /*!< The background Z-value, greater Z-value result in a pixmap shows above another one */
    static const double FOREGROUND_Z_VALUE; /*!< The foreground Z-value, greater Z-value result in a pixmap shows above another one */
    static const QString DEFAULT_PHOTO;    /*!< The default photo when no image is uploaded */
    QImage image; /*!< The image being processed for the scene */
    QGraphicsPixmapItem *background;  /*!< The background pixmap get from the original image after a series of process */
    QGraphicsPixmapItem *foreground;  /*!< The foreground pixmap get from the original image after a series of process */
    Graph* view;     /*!< The Graph paired with this GraphicsScene*/
    bool hasImage = false;  /*!< Determine if this GraphicsScene has an image*/

public slots:
    void sizeChange();

};

#endif // GraphicsScene_H

