#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <vector>

#include<QDebug>

enum HitorMiss{unknown, hit, miss};

enum direction{none,up,down,left_,right_};

namespace Ui {
class GameScreen;
}

class GameScreen : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief GameScreen Constructs GameScreen window
     * @param parent base class
     */
    explicit GameScreen(QWidget *parent = nullptr);
    ~GameScreen() override;
    /**
     * @brief paintEvent Paints board based on player actions
     * @param event
     */
    void paintEvent(QPaintEvent* event) override;
    void paintBoardLabels(QPainter& painter);

    /**
     * @brief checkIfDestroyed After shot hits, hides specific label if ship sunk and checks if game is over.
     */
    void checkIfDestroyed();

public slots:
    void setGrid(int player, const matrix& b);
    void setVersus();

private slots:
    void playerXWins(const int x);

private:
    Ui::GameScreen *ui;

    void mousePressEvent(QMouseEvent* event) override;

    ShipType player1Grid[10][10];//!<Keeps track of actual ship positions
    ShipType player2Grid[10][10];
    HitorMiss player1HitorMiss[10][10]; //!<Keeps track of what the player sees
    HitorMiss player2HitorMiss[10][10];

    int click_x = 0; //!<Keeps track of last mouse position
    int click_y = 0;

    int player1Ships[6]={0,5,4,3,3,2}; //!<Keeps track of player ships
    int player2Ships[6]={0,5,4,3,3,2};

    bool player1ShipsSunk[6]={1,0,0,0,0,0};

    bool versus = false; //!<Keeps track of mode

    int currentPlayer = 1;//!<Keeps track of current player for visual purposes

    QSoundEffect* explosion; //!<Plays when shot hit
    QSoundEffect* splash; //!<Plays when shot missed

    /**
     * @brief generateSearchShot Using only probability grid
     * @return
     */
    std::pair<int,int> generateSearchShot();

    /**
     * @brief adjustProbGrid Increases probability of adjacent squares if hit, decreases if miss
     * @param input Center square that was hit or miss
     */
    void adjustProbGrid(std::pair<int,int> input);


    int probGrid[10][10]; //!< Each element holds int value representing probability

    bool targetMode = false;
    bool lastShotSunk = false;
    std::pair<int,int> origSquare = std::make_pair(-1,-1);

    std::vector<std::pair<int,std::pair<int,int>>> turnBuffer;

    int lastX = 0;
    int lastY = 0;

    bool secondLastShotMissed = false;
    bool lastShotMissed = false;

    std::pair<int,int> generateTargetShot()
    {
        std::pair<int,int> rval = std::make_pair(0,0);

        if(lastShotMissed && secondLastShotMissed)
        {
            orientation=0;
            currDirection = none;
        }

        if (orientation==0)
        {
            if(turnBuffer.empty())
            {
//            qDebug()<<"Orientation is currently unknown.";

                //Search the 4 adjacent squares for highest probability.
                const int arrX[4] = {1,-1,0,0};
                const int arrY[4] = {0,0,-1,1};

                //right,left,up,down
                for (int i = 0; i < 4; ++i)
                {
                    int x = origSquare.first+arrX[i];
                    int y = origSquare.second+arrY[i];
                    if (x < 0 || x >= 10 || y < 0 || y >= 10)
                    {
                        continue;
                    }
                    turnBuffer.push_back(std::make_pair(probGrid[x][y],std::make_pair(x,y)));
                }
                std::sort(turnBuffer.begin(),turnBuffer.end());

//            for (int i = 0; i < 4; ++i) {
//                qDebug()<<turnBuffer[i].second.first<<","<<turnBuffer[i].second.second;
//            }
            }
            rval = turnBuffer[turnBuffer.size()-1].second;
            turnBuffer.pop_back();
        }
        else //if(orientation!=0)//ORIENTATION FOUND
        {
            qDebug()<<"ORIENTATION FOUND";

            bool done = false;
            int reverseCounter = 0;
            while(!done)
            {
                switch (currDirection) {
                case none:
                    qDebug()<<"ERROR: NO DIRECTION";
                    break;
                case up:
                    qDebug()<<"UP from LAST SHOT"<<lastX<<lastY;

                    rval = std::make_pair(lastX,lastY-1);

                    while (player2HitorMiss[rval.first][rval.second]==hit)
                    {
                        qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, UP AGAIN";
                        rval.second-=1;
                    }

                    if(player2HitorMiss[rval.first][rval.second]==miss)
                    {
                        qDebug()<<"UP AGAIN ENCOUNTERED MISS";
                        reverseCounter++;
                        if (reverseCounter == 2)
                        {
                            qDebug()<<"REVERSE COUNTER ==2";
                            orientation = 0;
                            reverseCounter = 0;
                            return generateTargetShot();
                        }
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                        rval = std::make_pair(lastX,lastY+1);
                    }

                    break;
                case down:
                    qDebug()<<"DOWN from LAST SHOT"<<lastX<<lastY;

                    rval = std::make_pair(lastX,lastY+1);

                    while (player2HitorMiss[rval.first][rval.second]==hit)
                    {
                        qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, DOWN AGAIN";
                        rval.second+=1;
                    }

                    if(player2HitorMiss[rval.first][rval.second]==miss)
                    {
                        qDebug()<<"DOWN AGAIN ENCOUNTERED MISS";
                        reverseCounter++;
                        if (reverseCounter == 2)
                        {
                            qDebug()<<"REVERSE COUNTER ==2";
                            orientation = 0;
                            reverseCounter = 0;
                            return generateTargetShot();
                        }
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                        rval = std::make_pair(lastX,lastY-1);
                    }

                    break;
                case left_:
                    qDebug()<<"LEFT from LAST SHOT"<<lastX<<lastY;

                    rval = std::make_pair(lastX-1,lastY);

                    while (player2HitorMiss[rval.first][rval.second]==hit)
                    {
                        qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, LEFT AGAIN";
                        rval.first-=1;
                    }

                    if(player2HitorMiss[rval.first][rval.second]==miss)
                    {
                        qDebug()<<"LEFT AGAIN ENCOUNTERED MISS";
                        reverseCounter++;
                        if (reverseCounter == 2)
                        {
                            qDebug()<<"REVERSE COUNTER ==2";
                            orientation = 0;
                            reverseCounter = 0;
                            return generateTargetShot();
                        }
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                        rval = std::make_pair(lastX+1,lastY);
                    }

                    break;
                case right_:
                    qDebug()<<"RIGHT from LAST SHOT:"<<lastX<<lastY;

                    rval = std::make_pair(lastX+1,lastY);

                    while (player2HitorMiss[rval.first][rval.second]==hit)
                    {
                        qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, RIGHT AGAIN";
                        rval.first+=1;
                    }

                    if(player2HitorMiss[rval.first][rval.second]==miss)
                    {
                        qDebug()<<"RIGHT AGAIN ENCOUNTERED MISS";
                        reverseCounter++;
                        if (reverseCounter == 2)
                        {
                            qDebug()<<"REVERSE COUNTER ==2";
                            orientation = 0;
                            reverseCounter = 0;
                            return generateTargetShot();
                        }
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                        rval = std::make_pair(lastX-1,lastY);
                    }

                    break;
                }
                if (rval.first < 0 || rval.first >= 10 || rval.second < 0 || rval.second >= 10)
                {
                    reverseCounter++;
                    if (reverseCounter == 2)
                    {
                        qDebug()<<"REVERSE COUNTER ==2";
                        orientation = 0;
                        reverseCounter = 0;
                        return generateTargetShot();
                    }
                    else {
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                    }
                }
                else {
                    done = true;
                }
            }
        }

        lastX = rval.first;
        lastY = rval.second;

        return rval;
    }

    void setOrigSquare(std::pair<int,int> newFirstHit)
    {
        qDebug()<<"SETTING NEW OG SQUARE";
        origSquare = newFirstHit;
        orientation = 0;
        currDirection = none;

    }

    int orientation = 0; //!<0,1,2 = unknown, horizontal, vertical
    direction currDirection = none;

    void reverseDirection()
    {
        switch (currDirection) {
        case none:
            qDebug()<<"ERROR: GAMESCREEN CPP, NPC MISS";
            break;
        case up:
            qDebug()<<"SWITCHED DIRECTION U->D";
            currDirection = down;
            break;
        case down:
            qDebug()<<"SWITCHED DIRECTION D->U";
            currDirection = up;
            break;
        case left_:
            qDebug()<<"SWITCHED DIRECTION L->R";
            currDirection = right_;
            break;
        case right_:
            qDebug()<<"SWITCHED DIRECTION R->L";
            currDirection = left_;
            break;
        }
    }
    QPixmap *leftPegMiss;
    QPixmap *leftPegHit;
    QPixmap *rightPegMiss;
    QPixmap *rightPegHit;
};

#endif // GAMESCREEN_H
