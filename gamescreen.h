#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QSoundEffect>

#include <utility>

#include<QDebug>

enum HitorMiss{unknown, hit, miss};

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

public slots:
    void setGrid(int player, const matrix& b);

private slots:
    void playerXWins(const int x);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    Ui::GameScreen *ui;

    /**
     * @brief checkIfDestroyed After shot hits, hides specific label if ship sunk and checks if game is over.
     */
    void checkIfDestroyed();

    ShipType player1Grid[10][10];//!<Keeps track of actual ship positions
    ShipType player2Grid[10][10];

    HitorMiss player1HitorMiss[10][10]; //!<Keeps track of what the player sees
    HitorMiss player2HitorMiss[10][10];

    int click_x = 0; //!<Keeps track of last mouse position
    int click_y = 0;

    int player1Ships[6]={0,5,4,3,3,2}; //!<Keeps track of player ships
    int player2Ships[6]={0,5,4,3,3,2};

    bool player1ShipsSunk[6]={1,0,0,0,0,0};

    int currentPlayer = 1;//!<Keeps track of current player for visual purposes

    bool lastShotSunk = false;

    QSoundEffect* explosion; //!<Plays when shot hit
    QSoundEffect* splash; //!<Plays when shot missed

    bool gameover = false;
private:
    void paintEvent(QPaintEvent* event) override;
    void paintBoardLabels(QPainter& painter);

    QPixmap *pegRed;
    QPixmap *pegWhite;
};

#endif // GAMESCREEN_H
