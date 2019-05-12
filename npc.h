#ifndef NPC_H
#define NPC_H

#include <QObject>
#include "ship.h"
#include "matrix.h"
#include <cstdlib>
#include <ctime>

class npc : public QObject
{
    Q_OBJECT
public:
    explicit npc();
    ~npc();

signals:
    void getdata(int x,const matrix& m) const;

public slots:
    void getnpc();

    const matrix& getdata();

private:
    bool placeRandom(Ship *newPiece, int newX, int newY);

    void randomize_board();

    ShipType grid[10][10]; //!< grid that contains ShipType values
    Ship *curShip = nullptr;

    int curX; //!< X coordinate of current position
    int curY; //!< Y coordinate of current position

    matrix *data = nullptr; //!<data to transfer
};

#endif // NPC_H
