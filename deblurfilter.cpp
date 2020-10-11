#include "deblurfilter.h"

/*!
 * \brief An overloaded mutator for the image-to-be-deblurred and the three residue images.
 * \param img The image to be deblurred
 * \param red The red residue image
 * \param green The green residue image
 * \param blue The blue residue image
 */
void DeblurFilter::setImage(QImage img, QImage red, QImage green, QImage blue) {
    image = img;
    redResidue = red;
    greenResidue = green;
    blueResidue = blue;
}

/*!
 * \brief An mutator for the size variable.
 *
 * This effectively changes the blurring strength of the filter.
 *
 * \param sz The new size selected by the image
 */
void DeblurFilter::setSize(int sz) {
    size = sz;
}

/*!
 * \brief Apply the filter to uploaded images.
 */
void DeblurFilter::apply() {
    cancelled = false;
    if (image.isNull()) {
        return;
    }

    // Initialize dynamic arrays
    int h = image.height();
    int w = image.width();
    double **r = new double*[h];
    double **g = new double*[h];
    double **b = new double*[h];
    for (int i = 0; i < h; i++) {
        r[i] = new double[w];
        g[i] = new double[w];
        b[i] = new double[w];
    }

    // Restore the red, green, and blue matrices from four images
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            QRgb pix = image.pixel(j, i);
            QRgb pixR = redResidue.pixel(j, i);
            QRgb pixG = greenResidue.pixel(j, i);
            QRgb pixB = blueResidue.pixel(j, i);
            r[i][j] = qRed(pix) + qGreen(pixR) / 100.0 + qBlue(pixR) / 10000.0
                    + qAlpha(pixR) / 1000000.0; // restore the double matrix containing averages of red color of the original image from the blurred image and the three residue images
            g[i][j] = qGreen(pix) + qRed(pixG) / 100.0 + qBlue(pixG) / 10000.0
                    + qAlpha(pixG) / 1000000.0; // restore the double matrix containing averages of green color of the original image from the blurred image and the three residue images
            b[i][j] = qBlue(pix) + qRed(pixB) / 100.0 + qGreen(pixB) / 10000.0
                    + qAlpha(pixB) / 1000000.0; // restore the double matrix containing averages of blue color of the original image from the blurred image and the three residue images
        }
    }

    // Execute deblurring for red, green, and blue matrices
    for (int x = 0; x < h; x += size) {
        emit(progressUpdated(x)); // communicate deblurring progress with MainWindow
        for (int y = 0; y < w; y += size) {
            if (cancelled) { // if user has pressed "Cancel", terminate immediately
                for (int i = 0; i < h; i++) {
                    delete[] r[i];
                    delete[] g[i];
                    delete[] b[i];
                }
                delete[] r;
                delete[] g;
                delete[] b;
                return;
            }
            matrix::deblurMatrix(r, h, w, x, y, size); // manipulate the red matrix with the method documented in matrix.cpp
            if (cancelled) {
                for (int i = 0; i < h; i++) {
                    delete[] r[i];
                    delete[] g[i];
                    delete[] b[i];
                }
                delete[] r;
                delete[] g;
                delete[] b;
                return;
            }
            matrix::deblurMatrix(g, h, w, x, y, size); // manipulate the green matrix with the method documented in matrix.cpp
            if (cancelled) {
                for (int i = 0; i < h; i++) {
                    delete[] r[i];
                    delete[] g[i];
                    delete[] b[i];
                }
                delete[] r;
                delete[] g;
                delete[] b;
                return;
            }
            matrix::deblurMatrix(b, h, w, x, y, size); // manipulate the blue matrix with the method documented in matrix.cpp
        }
    }

    // Construct the original image
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.setPixel(j, i,
                    qRgb(static_cast<int>(r[i][j]), static_cast<int>(g[i][j]),
                            static_cast<int>(b[i][j])));
        }
    }

    // Deallocate dynamic arrays
    for (int i = 0; i < h; i++) {
        delete[] r[i];
        delete[] g[i];
        delete[] b[i];
    }
    delete[] r;
    delete[] g;
    delete[] b;
    emit(progressUpdated(h)); // tell MainWindow that deblurring has finished
}
