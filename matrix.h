#ifndef MATRIX_H
#define MATRIX_H

#include "math.h"

/*!
 * \brief A class containing all useful matrix operations.
 */
class matrix {
public:
    matrix() = delete;
    static void blurMatrix(double **input, int h, int w, int x, int y,
            int size);
    static void deblurMatrix(double **input, int h, int w, int x, int y,
            int size);
    static void encode(int **to, int **from, int h, int w);
    static void decode(int **&to, int h, int w);
};

#endif // MATRIX_H
