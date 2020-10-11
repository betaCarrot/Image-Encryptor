#include "blurfilter.h"

/*!
 * \brief An overloaded mutator for the stored image and three residue images.
 *
 * All of the residue images are set to be the same size as the new image.
 * This is to prevent Index-out-of-Range errors when manipulating their pixels.
 *
 * \param img The new image uploaded by the user
 */
void BlurFilter::setImage(QImage img) {
    image = img;
    redResidue = img;
    greenResidue = img;
    blueResidue = img;
}

/*!
 * \brief An accessor for the red residue image.
 * \return The red residue image
 */
QImage BlurFilter::getRedResidue() {
    return redResidue;
}

/*!
 * \brief An accessor for the green residue image.
 * \return The green residue image
 */
QImage BlurFilter::getGreenResidue() {
    return greenResidue;
}

/*!
 * \brief An accessor for the blue residue image.
 * \return The blue residue image
 */
QImage BlurFilter::getBlueResidue() {
    return blueResidue;
}

/*!
 * \brief An mutator for the size variable.
 *
 * This effectively changes the blurring strength of the filter.
 *
 * \param sz The new size selected by the image
 */
void BlurFilter::setSize(int sz) {
    size = sz;
}

/*!
 * \brief Apply the filter to the uploaded image.
 */
void BlurFilter::apply() {
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

    // Extract the red, green, and blue information from pixels and put into 2D arrays.
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            QRgb pix = image.pixel(j, i);
            r[i][j] = qRed(pix);
            g[i][j] = qGreen(pix);
            b[i][j] = qBlue(pix);
        }
    }

    // Execute blurring for red, green, and blue matrices
    for (int x = 0; x < h; x += size) {
        emit(progressUpdated(x)); // communicate blurring progress with MainWindow
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
            matrix::blurMatrix(r, h, w, x, y, size); // manipulate the redmatrix with the method documented in matrix.cpp
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
            matrix::blurMatrix(g, h, w, x, y, size); // manipulate the green matrix with the method documented in matrix.cpp
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
            matrix::blurMatrix(b, h, w, x, y, size); // manipulate the blue matrix with the method documented in matrix.cpp
        }
    }

    // Put information in the double matrices into four images which can only store integer pixels
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.setPixel(j, i,
                    qRgb(static_cast<int>(r[i][j]), static_cast<int>(g[i][j]),
                            static_cast<int>(b[i][j]))); // let the blurred image contain the integer parts of the three matrices of doubles
            redResidue.setPixel(j, i,
                    qRgba(255, static_cast<int>(r[i][j] * 100) % 100,
                            static_cast<int>(r[i][j] * 10000) % 100,
                            static_cast<int>(r[i][j] * 1000000) % 100)); // the red residue image contains digits 1-6 after the decimal point of the red matrix
            greenResidue.setPixel(j, i,
                    qRgba(static_cast<int>(g[i][j] * 100) % 100, 255,
                            static_cast<int>(g[i][j] * 10000) % 100,
                            static_cast<int>(g[i][j] * 1000000) % 100)); // the green residue image contains digits 1-6 after the decimal point of the green matrix
            blueResidue.setPixel(j, i,
                    qRgba(static_cast<int>(b[i][j] * 100) % 100,
                            static_cast<int>(b[i][j] * 10000) % 100, 255,
                            static_cast<int>(b[i][j] * 1000000) % 100)); // the blue residue image contains digits 1-6 after the decimal point of the blue matrix
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
    emit(progressUpdated(h)); // tell MainWindow that blurring has finished
}
