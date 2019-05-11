#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
//#include <QMediaPlayer>
//#include <QSet>
//#include <QString>

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
    explicit GameScreen(QWidget *parent = 0);
    /**
     * @brief paintEvent Paints board based on player actions
     * @param event
     */

    void paintEvent(QPaintEvent* event) override;
    ~GameScreen() override;


    void paintBoardLabels(QPainter& painter);

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

public slots:
    void setGrid(int player, ShipType arr[10][10]);

private slots:
//    void playerXWins(const int& x);

private:
    Ui::GameScreen *ui;
    /**
     * @brief mousePressEvent Update mouse position if mouse is clicked
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override;

    ShipType player1Grid[10][10];
    ShipType player2Grid[10][10];
    HitorMiss player1HitorMiss[10][10];
    HitorMiss player2HitorMiss[10][10];

    int click_x = 0;
    int click_y = 0;

    int player1Ships[6]={0,5,4,3,3,2};
    int player2Ships[6]={0,5,4,3,3,2};

    int currentPlayer = 1;
};

#endif // GAMESCREEN_H
