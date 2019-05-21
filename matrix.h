#ifndef MATRIX_H
#define MATRIX_H

#include "ship.h"

class matrix
{
public:
    explicit matrix(const ShipType input[10][10]);
    void get_data(ShipType output[10][10]) const;
private:
    ShipType data[10][10];
};

#endif // MATRIX_H
