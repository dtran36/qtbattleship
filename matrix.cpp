#include "matrix.h"

matrix::matrix(const ShipType input[10][10])
{
    for (int i=0;i<10;i++) {
        for (int j=0;j<10;j++) {
            data[i][j]=input[i][j];
        }
    }
}

void matrix::get_data(ShipType output[10][10]) const
{
    for (int i=0;i<10;i++) {
        for (int j=0;j<10;j++) {
            output[i][j]=data[i][j];
        }
    }
}
