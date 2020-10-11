#include "graphicscene.h"

//const QString GraphicsScene::DEFAULT_PHOTO = ":";

const double GraphicsScene::BACKGROUND_Z_VALUE = -2000.0;

const double GraphicsScene::FOREGROUND_Z_VALUE = -1000.0;

/*!
 * \brief set the origional image into Graph's data member and process it to fill the scene.
 *
 * process the background image using the origional image and then process the foreground image using the origional image, add both into the scene
 *
 * \param img The image being processed and showed
 */
void GraphicsScene::setImage(const QImage &img) {
    if (img.isNull())
        return;
    image = img;
    hasImage = true;

    QImage enlargedImage = image.scaled(view->width(), view->height(),
            Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QImage choppedImage = enlargedImage.copy(0, 0, view->width(),
            view->height());
    // Foreground unprocessed image
    QImage scaledImage = image.scaled(view->width(), view->height(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (background) {
        this->removeItem(background);
        delete background;
    }
    background = this->addPixmap(QPixmap::fromImage(choppedImage));
    background->setTransformationMode(Qt::SmoothTransformation);
    background->setZValue(BACKGROUND_Z_VALUE);

    if (foreground) {
        this->removeItem(foreground);
        delete foreground;
    }
    foreground = this->addPixmap(QPixmap::fromImage(scaledImage));
    foreground->setPos(view->width() / 2.0 - scaledImage.width() / 2.0 - 1,
            view->height() / 2.0 - scaledImage.height() / 2.0 - 1);
    foreground->setTransformationMode(Qt::SmoothTransformation);
    background->setZValue(FOREGROUND_Z_VALUE);
}
;

/*!
 * \brief return the origional image in the scene.
 *
 * The function does what the name suggests
 *
 * \return return the origional image used to process the scene
 */

QImage GraphicsScene::getImage() const {
    return image;
}

/*!
 * \brief destructor: delete all the pointers in the class.
 *
 */
GraphicsScene::~GraphicsScene() {
    delete background;
    delete foreground;
    delete view;
}

/*!
 * \brief constructor: accept a Graph paired to this scene and create the GraphicsScene.
 *
 * This is a derived class of QGraphicsScene, we want to use it to add some more functions of GraphicsScene.
 *
 * \param view The Graph object paired with this GraphicsScene
 *
 * \param parent QObject parent
 */
GraphicsScene::GraphicsScene(Graph *view, QObject *parent) :
        QGraphicsScene(parent), background(nullptr), foreground(nullptr), view(
                view) {
    connect(view, &Graph::sizechange, this, &GraphicsScene::sizeChange);
}

/*!
 * \brief this is the slot to accept the sizechange signal, reset the scene to fit the GraphicsView.
 */

void GraphicsScene::sizeChange() {
    setImage (image);
    view->setScene(this);
}
