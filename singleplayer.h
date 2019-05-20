#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include <QObject>
#include "gamescreen.h"

#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

#include <QDebug>

enum direction{none,up,down,left_,right_};

class singleplayer : public GameScreen
{
    Q_OBJECT
public:
    singleplayer();

public slots:
//    void getOpponentData(int dummy, const matrix& opp);

private:
//randomize ship placement
    bool placeRandom(Ship *newPiece, int newX, int newY);
    void randomize_board();
    Ship *curShip = nullptr;
    int curX =0; //!< X coordinate of current position
    int curY =2; //!< Y coordinate of current position

//mouse event with AI as player 2
    virtual void mousePressEvent(QMouseEvent* event) override;
    /**
     * @brief generateSearchShot Using only probability grid
     * @return
     */
    std::pair<int,int> generateSearchShot();
    std::pair<int,int> generateTargetShot();
    /**
     * @brief adjustProbGrid Increases probability of adjacent squares if hit, decreases if miss
     * @param input Center square that was hit or miss
     */
    void adjustProbGrid(std::pair<int,int> input);
    void setOrigSquare(std::pair<int,int> newFirstHit);
    void reverseDirection();


    int probGrid[10][10]; //!< Each element holds int value representing probability
    std::pair<int,int> origSquare = std::make_pair(-1,-1);
    bool targetMode = false;

    int orientation = 0; //!<0,1,2 = unknown, horizontal, vertical
    direction currDirection = none;
    int reverseCounter = 0;

    bool secondLastShotMissed = false;
    bool lastShotMissed = false;

    int lastX = 0;
    int lastY = 0;
    std::vector<std::pair<int,std::pair<int,int>>> turnBuffer;
};

#endif // SINGLEPLAYER_H
