#include "npc.h"

NPC::NPC()
{
    //set all elements of grid to empty ShipType
    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            grid[i][j]=empty;
        }
    }

    srand(time(0));

    //set initial ship placement position
    curX=0;
    curY=2;

    randomize_board();
}
NPC::~NPC()
{
    delete data;
}

void NPC::getNPC()
{
    //convert grid to matrix
    data = new matrix(grid);
    emit giveData(2,*data);
}


bool NPC::placeRandom(Ship *newPiece, int newX, int newY)
{
    for (int i = 0; i < 5; ++i)
    {
        int x = newX + newPiece->x(i);
        int y = newY - newPiece->y(i);

        // if X or Y is not within 0-9, or square is taken
        if (x < 0 || x >= 10 || y < 0 || y >= 10 || grid[x][y]!=empty)
            return false; //new position ship is out of bounds

        // strategic placement: no ships are touching
        int arrX[4] = {x+1,x-1,x,x};
        int arrY[4] = {y,y,y+1,y-1};
        for (int j =0;j<4;j++) {
            if (grid[arrX[j]][arrY[j]]!=newPiece->getType() && grid[arrX[j]][arrY[j]]!=empty)
                return false;
        }
    }

    // new position ship is within grid bounds
    curX = newX; // set new curX
    curY = newY; // set new curY
    return true;
}

void NPC::randomize_board()
{
    //randomize orientation
    bool horizontal[6]; // true for horizontal
    for (int i =0;i<6;i++)
    {
        horizontal[i] = rand() % 2;
    }
    const ShipType refarr[6] = {empty,carrier,battleship,submarine,destroyer,patrol};

    //randomize placement
    for (int i =1; i<6;i++)
    {
        curShip = new Ship(refarr[i]);
        if(horizontal[i])
            curShip->rotate();

        bool placed = false;
        int randX = 0;
        int randY = 0;

        //randomize ship's grid position
        while(!placed)
        {
            randX = rand() % 10;
            randY = rand() % 10;
            placed=placeRandom(curShip,randX, randY);
        }

        //save the position of the squares
        for(int i =0; i<5; i++)
        {
            grid[curX + curShip->x(i)][curY - curShip->y(i)]=curShip->getType();
        }

        delete curShip;
    }
}
