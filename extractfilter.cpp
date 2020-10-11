#include "extractfilter.h"

/*!
 * \brief Apply the filter to uploaded images.
 */
void ExtractFilter::apply() {
    movie->jumpToNextFrame();

    // Locate the correct frame to be extracted
    int location = 3;
    for (int i = 0; i < location; i++) {
        movie->jumpToNextFrame();
    }

    // Set the correct size and format for the output image
    QImage image2 { movie->currentImage().size(), QImage::Format_RGBA8888 };
    image = image2;

    // Construct the original image
    for (int i = 0; i < image.height(); i++) {
        for (int j = 0; j < image.width(); j++) {
            image.setPixel(j, i, movie->currentImage().pixel(j, i));
        }
    }
}
