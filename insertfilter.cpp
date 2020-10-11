#include "insertfilter.h"

/*!
 * \brief An overloaded mutator for the movie variable and three residue images.
 *
 * Also parses the individual frames of the gif and stores them into the linked list.
 *
 * \param mov The gif image uploaded by the user
 */
void InsertFilter::setMovie(QMovie *mov) {
    list.clear();
    movie = mov;
    mov->jumpToNextFrame();
    for (int i = 0; i < mov->frameCount(); i++) {
        list.append(mov->currentImage());
        mov->jumpToNextFrame();
    }
}

/*!
 * \brief A mutator for the destination variable.
 * \param path The folder specified by the user
 */
void InsertFilter::setDestination(QString path) {
    destination = path;
}

/*!
 * \brief An accessor for the destination variable.
 * \return The folder previously specified by the user
 */
QString InsertFilter::getDestination() {
    return destination;
}
/*!
 * \brief Apply the filter to uploaded images.
 */
void InsertFilter::apply() {
    movie->jumpToNextFrame();

    // Msake the secret image exactly the same size as the frames of gif to avoid index-out-of-bounds error
    image = image.scaled(movie->currentImage().size(), Qt::KeepAspectRatio); // scale the secret image to match the size of gif
    QImage image2 { movie->currentImage().size(), QImage::Format_RGBA8888 }; // create a template image of the desired size
    for (int i = 0; i < movie->currentImage().height(); i++) {
        for (int j = 0; j < movie->currentImage().width(); j++) {
            if (i < image.height() && j < image.width()) {
                image2.setPixel(j, i, image.pixel(j, i));
            } else {
                image2.setPixel(j, i, qRgba(0, 0, 0, 0)); // set the region outside the secret image to be
            }
        }
    }
    image = image2;

    // Insert the secret image into the linked list
    int location = 3;
    list.insert(list.begin() + location, image);

    // Define some variables
    QVector < uint8_t > vector;
    int height = movie->currentImage().height();
    int width = movie->currentImage().width();
    int delay = movie->nextFrameDelay() / 10; // divide by 10 since Qt counts in milliseconds but gif.h counts in hundredth of a second

    // Use external library gif.h to build a gif image from individual frames
    GifWriter g;
    char *fileName = destination.toLocal8Bit().data();
    GifBegin(&g, fileName, width, height, delay);
    QLinkedList<QImage>::iterator iter = list.begin(); // linked list iterator
    for (int i = 0; i < list.size(); i++, iter++, movie->jumpToNextFrame()) {
        vector.clear();
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                QRgb pix = iter->pixel(k, j);
                vector.append(static_cast<uint8_t>(qRed(pix)));
                vector.append(static_cast<uint8_t>(qGreen(pix)));
                vector.append(static_cast<uint8_t>(qBlue(pix)));
                vector.append(static_cast<uint8_t>(qAlpha(pix)));
            }
        }
        GifWriteFrame(&g, vector.data(), width, height, delay);
    }
    GifEnd(&g);
}
