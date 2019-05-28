#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <vector>
#include <QCursor>

#include<QDebug>

enum HitorMiss{unknown, hit, miss};

enum direction{none,up,down,left_,right_};

namespace Ui {
class GameScreen;
}

class GameScreen : public QFrame
{
    Q_OBJECT
signals:
    /**
     * @brief playerXWins Emitted when all ships all sunk.
     * @param x player that won, 1 or 2
     */
    void playerXWins(const int x);

public:
    /**
     * @brief GameScreen Constructs GameScreen window
     * @param parent base class
     */
    explicit GameScreen(QWidget *parent = nullptr);
    ~GameScreen() override;


public slots:
    /**
     * @brief setGrid Triggered when SetupScreen or NPC emits data.
     * @param player to set grid as
     * @param b the matrix that is being copied
     */
    void setGrid(int player, const matrix& b);

    /**
     * @brief setVersus set versus member data as true
     */
    void setVersus();

private slots:
    void on_leftSpecial_clicked();

    void on_rightSpecial_clicked();

private:
    Ui::GameScreen *ui;

    /**
     * @brief paintEvent Paints board based on player actions
     * @param event
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief paintBoardLabels Paints letters, numbers, and lines of the 2 grids
     * @param painter object to use
     */
    void paintBoardLabels(QPainter& painter);

    /**
     * @brief checkIfDestroyed Called after shot hits, checks if ship sunk, checks if game is over.
     */
    void checkIfDestroyed();

    /**
     * @brief mousePressEvent Processes main functions that occur after mouse input, eg. hit or miss.
     * @param event
     */
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

//MEMBERS FOR AI ALGORITHM BELOW
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

    bool targetMode = false; //!<Keeps track of whether targetting a ship or trying to find new target
    bool lastShotSunk = false; //!<Keeps track if the last shot sunk a ship

    std::pair<int,int> origSquare = std::make_pair(-1,-1);//!<Keeps track of the original square to target

    std::vector<std::pair<int,std::pair<int,int>>> turnBuffer;//!<Used to find the orientation of a new target

    int lastX = 0; //!<Keeps track of last shot X coordinate
    int lastY = 0; //!<Keeps track of last shot Y coordinate

    bool secondLastShotMissed = false;//!<Keeps track if second to last shot missed
    bool lastShotMissed = false;//!<Keeps track if last shot missed

    /**
     * @brief generateTargetShot Generates a shot aiming to sink the current target ship.
     * @return the shot coordinates
     */
    std::pair<int,int> generateTargetShot();

    /**
     * @brief setOrigSquare If a square is hit while target mode is off, sets new original square.
     * @param newFirstHit the new hit coordinates
     */
    void setOrigSquare(std::pair<int,int> newFirstHit);

    int orientation = 0; //!<0,1,2 = unknown, horizontal, vertical
    direction currDirection = none; //!<keeps track of current direcition to aim from the original target, directions defined in enum

    int reverseCounter = 0; //!<Keeps track of how many times reverse has been called, if >2 orientation is flipped

    /**
     * @brief reverseDirection Flips the currDirection in the opposite direction
     */
    void reverseDirection();

    QPixmap pegRed; //!<Image for hit peg
    QPixmap pegWhite; //!<Image for miss peg

    QPixmap pixmapCursor;
    QCursor cursorSpecial;

    int requiredShotsPlayer1 = 2;
    int requiredShotsPlayer2 = 2;

    int player1Consec = 0;
    int player2Consec = 0;

    bool player1Special = false;
    bool player2Special = false;

    bool player1NextMoveSpecial = false;
    bool player2NextMoveSpecial = false;
};

#endif // GAMESCREEN_H
