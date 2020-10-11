#include "matrix.h"

/*!
 * \brief For each element in a certain region of a 2D array, replace its value with the average of all the values that are within a certain Manhattan distance from it (including the element itself).
 *
 * This distance is set to be half of the size of the region.
 * Such a distance is large enough to maximize the blurring effect and small enough to guarantee unique solution during row-reduction.
 *
 * For example, in a 6*6 matrix, the upper-left corner of a matrix is replaced with the average of all elements marked with 1.
 *       1 1 1 1 0 0
 *       1 1 1 0 0 0
 *       1 1 0 0 0 0
 *       1 0 0 0 0 0
 *       0 0 0 0 0 0
 *       0 0 0 0 0 0
 *
 * \param input The 2D array
 * \param h The number of rows
 * \param w The number of columns
 * \param x The row-index of the left-most corner of the region
 * \param y The column-index of the left-most corner of the region
 * \param size The size of the region
 */
void matrix::blurMatrix(double **input, int h, int w, int x, int y, int size) {
    int xSize = min(size, h - x); // resize the height of the selected region to avoid accessing non-existent pixels
    int ySize = min(size, w - y); // resize the width of the selected region to avoid accessing non-existent pixels
    int n = xSize * ySize;
    int d = min(xSize / 2, ySize / 2); // set the appropriate Manhattan distance

    // Initialize dynamic arrays
    double **a = new double*[n];
    for (int i = 0; i < n; i++) {
        a[i] = new double[n + 1];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            a[i][j] = 0;
        }
    }

    // Perform blurring
    for (int i = x; i < x + xSize; i++) {
        for (int j = y; j < y + ySize; j++) {
            {
                a[(i - x) * ySize + (j - y)][n] = input[i][j];
                int cnt = 0;
                double sum = 0.0;
                for (int ii = x; ii < x + xSize; ii++) {
                    for (int jj = y; jj < y + ySize; jj++) {
                        if (abs(i - ii) + abs(j - jj) <= d) { // the element (ii,jj) is within d units away from the current element (i,j)
                            cnt++;
                            sum += input[ii][jj];
                        }
                    }
                }
                a[(i - x) * ySize + (j - y)][n] = sum / cnt; // set the value of current element to be the average
            }
        }
    }

    // Put result values back into input matrices
    for (int i = x; i < min(h, x + size); i++) {
        for (int j = y; j < min(w, y + size); j++) {
            input[i][j] = a[(i - x) * ySize + (j - y)][n];
        }
    }

    // Deallocate dynamic arrays
    for (int i = 0; i < n; i++) {
        delete[] a[i];
    }
    delete[] a;
}

/*!
 * \brief The reverse process of blurring: Given a 2D array processed by the blurMatrix function, find the original array with row-reduction algorithm.
 *
 * For example, since the upper-left element is the average of 10 elements, we have the following equation:
 * x1 + x2 + x3 + x4 + x7 + x8 + x9 + x13 + x14 + x19 = input[0][0] * 10
 * where x(i*6+j) denotes the original value of element (i,j) before blurring.
 *
 * We can write a total of 36 equations for all 36 elements.
 * These 36 equations have exactly 36 varaibles.
 *
 * Write these equations as a 36*37 matrix, and do row-reduction.
 * It is mathematically proven that there will be a unique solution.
 *
 * \param input The 2D array
 * \param h The number of rows
 * \param w The number of columns
 * \param x The row-index of the left-most corner of the region
 * \param y The column-index of the left-most corner of the region
 * \param size The size of the region
 */
void matrix::deblurMatrix(double **input, int h, int w, int x, int y,
        int size) {
    int xSize = min(size, h - x); // resize the height of the selected region to avoid accessing non-existent pixels.
    int ySize = min(size, w - y); // resize the width of the selected region to avoid accessing non-existent pixels.
    int n = xSize * ySize;
    int d = min(xSize / 2, ySize / 2); // set the appropriate Manhattan distance

    // Initialize dynamic arrays
    double **a = new double*[n];
    for (int i = 0; i < n; i++) {
        a[i] = new double[n + 1];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            a[i][j] = 0;
        }
    }

    // Construct matrix
    for (int i = x; i < min(h, x + size); i++) {
        for (int j = y; j < min(w, y + size); j++) {
            {
                a[(i - x) * ySize + (j - y)][n] = input[i][j];
                int cnt = 0;
                for (int ii = x; ii < min(h, x + size); ii++) {
                    for (int jj = y; jj < min(w, y + size); jj++) {
                        if (abs(i - ii) + abs(j - jj) <= d) { // the element (ii,jj) is within d units away from the current element (i,j)
                            cnt++;
                            a[(i - x) * ySize + (j - y)][(ii - x) * ySize
                                    + (jj - y)] = 1; // set the corresponding entry in the matrix to be 1
                        }
                    }
                }
                a[(i - x) * ySize + (j - y)][n] *= cnt; // set one of the values in the rightmost column of the matrix
            }
        }
    }

    // Perform row-reduction algorithm
    math::rref(a, n, n + 1);

    // Put result values back into input matrices
    for (int i = x; i < min(h, x + size); i++) {
        for (int j = y; j < min(w, y + size); j++) {
            input[i][j] = a[(i - x) * ySize + (j - y)][n];
        }
    }

    // Deallocate dynamic arrays
    for (int i = 0; i < n; i++) {
        delete[] a[i];
    }
    delete[] a;
}

/*!
 * \brief Hide the most significant four bits of each element in the source 2D array into the least significant four bits of the corresponding element in the destination 2D array.
 *
 * We simply call the math::insert function for all elements in the 2D array.
 *
 * \param to The destination matrix
 * \param from The source matrix
 * \param h The number of rows
 * \param w The number of columns
 */
void matrix::encode(int **to, int **from, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            math::insert(to[i][j], from[i][j]);
        }
    }
}

/*!
 * \brief For each element in a 2D array, replace its value with the value of its most significant four bits.
 *
 * We simply call the math::insert function for all elements in the 2D array.
 *
 * \param to The matrix being processed
 * \param h The number of rows
 * \param w The number of columns
 */
void matrix::decode(int **&to, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            to[i][j] = math::extract(to[i][j]);
        }
    }
}
