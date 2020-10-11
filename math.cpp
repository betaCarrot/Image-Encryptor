#include "math.h"

/*!
 * \brief Check is a double value is actually zero.
 * \param x The value to be checked.
 * \return Whether the value is actually zero or not.
 */
bool math::zero(double x) {
    return abs(x) <= EPS;
}

/*!
 * \brief Set the element at index j of an array to be zero and scale the rest of the array.
 * \param v The array
 * \param size The size of the array
 * \param j The index of the element
 */
void math::normalize(double *v, int size, int j) {
    double x = v[j];
    for (int k = j; k < size; k++) {
        v[k] /= x;
    }
}

/*!
 * \brief Scale the current row with respect to the pivot row.
 * \param v The pivot row
 * \param u The current row being processed
 * \param size The size of the two rows
 * \param j The pivot column being processed
 */
void math::scale(double *v, double *u, int size, int j) {
    if (zero(u[j]))
        return;
    double r = u[j] / v[j]; // get the scaling factor
    for (int k = j; k < size; k++) {
        u[k] -= r * v[k]; // scale all elemnts of the current row to the right of the pivot
    }
}

/*!
 * \brief Perform row-reduction algorithm on a 2D matrix.
 * \param a The 2D matrix
 * \param n The number of rows
 * \param m The number of columns
 */
void math::rref(double **a, int n, int m) {
    int r = 0;
    for (int j = 0; j < m; j++) {
        bool found = false;
        for (int i = r; i < n; i++) {
            if (!zero(a[i][j])) {
                found = true;
                swap(a[r], a[i]); // set the first entry of the column to be non-zero
                break;
            }
        }
        if (!found)
            continue; // all entires of the column are zero
        normalize(a[r], n + 1, j); // set the pivot to be 1
        for (int i = 0; i < n; i++) {
            if (!zero(a[i][j]) && i != r)
                scale(a[r], a[i], n + 1, j); // scale other rows with respect to pivot row
        }
        r++;
    }
}

/*!
 * \brief Insert the first four bits of one integer into the last four bits of another integer,
 * \param to The destination integer.
 * \param from The source integer.
 */
void math::insert(int &to, const int &from) {
    to &= 0b11110000; // wipe clean the last four bits of the destination integer
    for (int i = 7; i >= 4; i--) {
        if ((from >> i) & 1) {
            to |= (1 << (i - 4)); // insert bits one-by-one
        }
    }
}

/*!
 * \brief Extract the last four bits of an integer.
 * \param cipher The given integer
 * \return The last four bits of the given integer
 */
int math::extract(int cipher) {
    int res = 0;
    for (int i = 3; i >= 0; i--) {
        if ((cipher >> i) & 1)
            res |= (1 << (i + 4)); // extract bits one-by-one
    }
    return res;
}
