#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "setupscreen.h"

#include <QFrame>
#include <QPainter>
#include <QMouseEvent>
#include <QMediaPlayer>

#include <QSoundEffect>

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
};

#endif // GAMESCREEN_H
