#include "ship.h"

Ship::Ship(ShipType t){
    type = t;
    setVertical();
}

ShipType Ship::getType() const { return type; }

int Ship::x(int index) const { return coords[index][0]; }
int Ship::y(int index) const { return coords[index][1]; }

void Ship::setVertical(){
    vertical = true;
    static const int coordsTable[6][5][2] = {
        { { 0, 0},   { 0, 0 },  { 0, 0 },   { 0, 0 }, { 0, 0 } }, // empty
        { { 0, -2 },  { 0, -1 },   { 0, 0 },   { 0, 1 },{0,2} }, // carrier
        { { 0, 0},   { 0, -1},  { 0, 0 },   { 0, 1 }, { 0, 2}  }, // battleship
        { { 0, 0},   { 0, -1},  { 0, 0 },   { 0, 1 }, { 0, 0 } }, // submarine
        { { 0, 0},   { 0, -1},  { 0, 0 },   { 0, 1 }, { 0, 0 } }, // destroyer
        { { 0, 0},   { 0, 0},  { 0, 0},   { 0, 1 }, { 0, 0 } }  // patrol
    };

    for (int i=0;i<5;i++) //index of the square
    {
        for(int j=0; j<2 ;j++) //X value, Y value
        {
            coords[i][j] = coordsTable[type][i][j];
        }
    }
}

void Ship::setHorizontal(){
    vertical = false;
    static const int coordsTable[6][5][2] = {
        { { 0, 0},   { 0, 0 },  { 0, 0 },   { 0, 0 }, { 0, 0 } }, // empty
        { { 2, 0 },  { 1, 0 },   { 0, 0 },   { -1, 0 },{-2,0} }, // carrier
        { { 0, 0},   { 1, 0},  { 0, 0 },   { -1, 0 }, { -2, 0}  }, // battleship
        { { 0, 0},   { 1, 0},  { 0, 0 },   { -1, 0 }, { 0, 0 } }, // submarine
        { { 0, 0},   { 1, 0},  { 0, 0 },   { -1, 0 }, { 0, 0 } }, // destroyer
        { { 0, 0},   { 0, 0},  { 0, 0 },   { 1, 0 }, { 0, 0 } }  // patrol
    };

    for (int i=0;i<5;i++) //index of the square
    {
        for(int j=0; j<2 ;j++) //X value, Y value
        {
            coords[i][j] = coordsTable[type][i][j];
        }
    }
}

void Ship::rotate(){
    if (vertical) // if ship is vertical
    {
        setHorizontal();
        return;
    }
    setVertical();
}

void Ship::setX(int index, int x) { coords[index][0] = x; }
void Ship::setY(int index, int y) { coords[index][1] = y; }
