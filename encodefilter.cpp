#include "encodefilter.h"

/*!
 * \brief A mutator of the the secret variable.
 * \param img The secret image uploaded by the uesr
 */
void EncodeFilter::setSecret(QImage img) {
    secret = img;
}

/*!
 * \brief Apply the filter to uploaded images.
 */
void EncodeFilter::apply() {
    if (image.isNull())
        return;

    // Scale the original image to be large enough to cover the entire secret image
    image = image.scaled(secret.size(), Qt::KeepAspectRatioByExpanding);

    // The region of source image containing the secret image will have degraded quality.
    // So we degrade all of the source image first to avoid sharp contrast between the region with secret information and the region without
    for (int i = 0; i < image.height(); i++) {
        for (int j = 0; j < image.width(); j++) {
            image.setPixel(j, i,
                    qRgb(qRed(image.pixel(j, i)) & 0b11110000,
                            qGreen(image.pixel(j, i)) & 0b11110000,
                            qBlue(image.pixel(j, i)) & 0b11110000));
        }
    }

    // Initialize dynamic arrays
    int h = secret.height();
    int w = secret.width();
    int **r = new int*[h];
    int **g = new int*[h];
    int **b = new int*[h];
    int **r2 = new int*[h];
    int **g2 = new int*[h];
    int **b2 = new int*[h];
    for (int i = 0; i < h; i++) {
        r[i] = new int[w];
        g[i] = new int[w];
        b[i] = new int[w];
        r2[i] = new int[w];
        g2[i] = new int[w];
        b2[i] = new int[w];
    }

    // Extract the red, green, and blue information from pixels and put into 2D arrays.
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            QRgb pix = image.pixel(j, i);
            QRgb pix2 = secret.pixel(j, i);
            r[i][j] = qRed(pix);
            g[i][j] = qGreen(pix);
            b[i][j] = qBlue(pix);
            r2[i][j] = qRed(pix2);
            g2[i][j] = qGreen(pix2);
            b2[i][j] = qBlue(pix2);
        }
    }

    // Manipulate the red, green, and blue matrices with the method documented in matrix.cpp
    matrix::encode(r, r2, h, w);
    matrix::encode(g, g2, h, w);
    matrix::encode(b, b2, h, w);

    // Construct the cipher image
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.setPixel(j, i, qRgb(r[i][j], g[i][j], b[i][j]));
        }
    }

    // Deallocate dynamic arrays
    for (int i = 0; i < h; i++) {
        delete[] r[i];
        delete[] r2[i];
        delete[] g[i];
        delete[] g2[i];
        delete[] b[i];
        delete[] b2[i];
    }
    delete[] r;
    delete[] r2;
    delete[] g;
    delete[] g2;
    delete[] b;
    delete[] b2;
}
