#ifndef NPC_H
#define NPC_H

#include <QObject>
#include "ship.h"
#include "matrix.h"
#include <cstdlib>
#include <ctime>
#include <utility>

class NPC : public QObject
{
    Q_OBJECT

signals:
    /**
     * @brief giveData Emitted by getNPC.
     * @param x player to set grid as
     * @param m the matrix to send
     */
    void giveData(int x,const matrix& m) const;

public:
    explicit NPC();
    ~NPC();

public slots:
    /**
     * @brief getNPC Triggered when signal to get NPC data is detected.
     */
    void getNPC();

private:
    /**
     * @brief placeRandom Tests if a current ship fits in to new random position.
     * @param newPiece current ship trying to place
     * @param newX new X position to try
     * @param newY new Y position to try
     * @return true if successful random placement in the new coordinates
     */
    bool placeRandom(Ship *newPiece, int newX, int newY);

    /**
     * @brief randomize_board Randomly orients and places ships.
     */
    void randomize_board();

    ShipType grid[10][10]; //!< grid that contains ShipType values
    Ship *curShip = nullptr; //!< pointer used in ship placement

    int curX; //!< X coordinate used in ship placement
    int curY; //!< Y coordinate used in ship placement

    matrix *data = nullptr; //!<data to transfer
};

#endif // NPC_H
