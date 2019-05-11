#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>

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
    /**
     * @brief paintEvent Paints board based on player actions
     * @param event
     */

    void paintEvent(QPaintEvent* event) override;
    ~GameScreen() override;

    void paintBoardLabels(QPainter& painter);
    void checkIfDestroyed();

public slots:
    void setGrid(int player, const matrix& b);

private slots:
    void playerXWins(const int x);

private:
    Ui::GameScreen *ui;

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
