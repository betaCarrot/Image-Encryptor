#ifndef MATH_H
#define MATH_H

#include <algorithm>

using namespace std;

const double EPS = 1e-8;

/*!
 * \brief A class containing all useful math functions.
 */
class math {
public:
    math() = delete;
    static void rref(double **a, int n, int m);
    static void insert(int &to, const int &from);
    static int extract(int cipher);
private:
    static bool zero(double x);
    static void normalize(double *v, int size, int j);
    static void scale(double *v, double *u, int size, int j);
};

#endif // MATH_H
