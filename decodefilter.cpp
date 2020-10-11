#include "decodefilter.h"

/*!
 * \brief Apply the filter to uploaded image.
 */
void DecodeFilter::apply() {
    if (image.isNull()) {
        return;
    }

    // Initialize dynamic arrays
    int h = image.height();
    int w = image.width();
    int **r = new int*[h];
    int **g = new int*[h];
    int **b = new int*[h];
    for (int i = 0; i < h; i++) {
        r[i] = new int[w];
        g[i] = new int[w];
        b[i] = new int[w];
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

    // Manipulate the red, green, and blue matrices with the method documented in matrix.cpp
    matrix::decode(r, h, w);
    matrix::decode(g, h, w);
    matrix::decode(b, h, w);

    // Construct the original image
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.setPixel(j, i, qRgb(r[i][j], g[i][j], b[i][j]));
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
}
