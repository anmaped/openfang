/* -*- Mode: C++ -*- */
#ifndef __WVMATRIX_H
#define __WVMATRIX_H

#include "wvstring.h"

class WvMatrix
{
public:
    int *data;

    int m;
    int n;

    WvMatrix(const int _m, const int _n, const int *_data = 0);
    ~WvMatrix();

    WvMatrix(const WvMatrix& mx);
    WvMatrix& operator= (const WvMatrix& mx);

    int& operator() (const int row, const int col)
        { return data[n*row + col]; }
    int operator() (const int row, const int col) const
        { return data[n*row + col]; }

    WvMatrix operator+ (const WvMatrix &rhs) const;
    WvMatrix operator* (const WvMatrix &rhs) const;

    WvString printable();
};

#endif // __WVMATRIX_H
