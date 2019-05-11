#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include <QMediaPlayer>
#include <QSet>
#include <QString>

enum HitorMiss{blank, hit, miss};

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
    explicit GameScreen(QWidget *parent = 0);
    /**
     * @brief paintEvent Paints board based on player actions
     * @param event
     */
    void paintEvent(QPaintEvent* event) override;
    ~GameScreen() override;



    /**
     * @brief paintText Paints board column and row text
     * @param painter
     * @param x_coord x position to start painting
     * @param y_coord y position to start painting
     */
    void paintText(QPainter& painter, int x_coord, int y_coord);
    /**
     * @brief checkIfDestroyed Checks if players' ships are destroyed
     */
    void checkIfDestroyed();
    /**
     * @brief updateShips If players' ships are destroyed, paint red. Else paint black
     * @param painter
     * @param x_coord x position to start painting
     * @param y_coord y position to start painting
     */
    void updateShips(QPainter& painter, int x_coord, int y_coord);
    /**
     * @brief paintBoard Paints players' actions (Hit or Miss)
     * @param painter
     */
    void paintBoard(QPainter& painter);
    /**
     * @brief gameplay Players take turn marking squares. Hit - go again, Miss - next player
     * @param painter
     */
    void gameplay(QPainter& painter);
    /**
     * @brief updateHits If Hit, then update which ship was hit
     * @param hit_ship
     */
    void updateHits(ShipType hit_ship);

//    void checkIfWinner();

public slots:
    void setBoard(int playerx, ShipType arr[10][10]);

private slots:
    /**
     * @brief soundUpdate Delays paint of the computer's actions
     */
    void soundUpdate();


private:
    Ui::GameScreen *ui;
    /**
     * @brief mousePressEvent Update mouse position if mouse is clicked
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override;

    ShipType player1Grid[10][10];
    ShipType player2Grid[10][10];
    HitorMiss player1HitMiss[10][10];
    HitorMiss player2HitMiss[10][10];

    int click_x = 0;
    int click_y = 0;

    // Player 1 Info
    bool player1_carrier_destroyed = false;
    bool player1_battleship_destroyed = false;
    bool player1_submarine_destroyed = false;
    bool player1_destroyer_destroyed = false;
    bool player1_patrol_destroyed = false;

    int player1_carrier_hit = 0;
    int player1_battleship_hit = 0;
    int player1_submarine_hit = 0;
    int player1_destroyer_hit = 0;
    int player1_patrol_hit = 0;

    // Player 2 Info
    bool player2_carrier_destroyed = false;
    bool player2_battleship_destroyed = false;
    bool player2_submarine_destroyed = false;
    bool player2_destroyer_destroyed = false;
    bool player2_patrol_destroyed = false;

    int player2_carrier_hit = 0;
    int player2_battleship_hit = 0;
    int player2_submarine_hit = 0;
    int player2_destroyer_hit = 0;
    int player2_patrol_hit = 0;

    // Computer Info
    bool is_computer = true;
    int rand_x_grid_pos;
    int rand_y_grid_pos;
    int origin_x;
    int origin_y;
    int rand_direction = 0;
    int continue_direction;
    bool successful_second_hit = false;
    int squaresHit = 0;

    bool player1_turn = true;
    bool no_winner = true;


    QSet<QString> destroyedShips;
    size_t initialSize;

    QMediaPlayer* explosion;
    QMediaPlayer* splash;

    int count = 0;

//    void enterEvent(QEvent *event);
//    void leaveEvent(QEvent *event);
};

#endif // GAMESCREEN_H
