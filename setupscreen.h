#ifndef SETUPSCREEN_H
#define SETUPSCREEN_H

#include<QFrame>
#include<QPainter>
#include<QString>
#include<QKeyEvent>
#include<QPushButton>
#include"ship.h"

#include"matrix.h"

namespace Ui {
class SetupScreen;
}

class SetupScreen : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief SetupScreen Constructs hidden QDialog window.
     * @param x player 1,2 to set QPixmap image
     * @param parent
     */
    explicit SetupScreen(int x=1,QWidget *parent = nullptr);
    ~SetupScreen();


signals:
    /**
     * @brief moveNext All ships placed, move to next widget.
     */
    void moveNext(int playerx, const matrix& m);

private slots:
    void on_pushCarrier_clicked();
    void on_pushBattleship_clicked();
    void on_pushSubmarine_clicked();
    void on_pushDestroyer_clicked();
    void on_pushPatrol_clicked();

    /**
     * @brief confirmPlacement Actual function that confirms current ship placement.
     */
    void confirmPlacement();

    /**
     * @brief cancelPlacement Actual function that cancels current ship placement.
     */
    void cancelPlacement();

    /**
     * @brief Acts when confirm button is clicked.
     */
    void on_pushButton_clicked();

    /**
     * @brief Acts when cancel button is clicked.
     */
    void on_pushButton_2_clicked();

private:
    Ui::SetupScreen *ui;

    /**
     * @brief paintEvent Paints board letters and numbers, current ship position, and placed ships.
     * @param event
     */
    void paintEvent(QPaintEvent* event);

    /**
     * @brief keyPressEvent If arrow keys or R key is pressed, curX, curY is changed accordingly and repaint is called.
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief drawSquare Draws a square at the given top,left coordinates.
     * @param painter the existing QPainter object to use
     * @param x top coordinate of the square
     * @param y left coordinate of the square
     */
    void drawSquare(QPainter &painter, int x, int y);

    ShipType grid[10][10]; //!< grid that contains ShipType values

    /**
     * @brief clearGrid Sets all values of the grid to ShipType::empty.
     */
    void clearGrid();

    Ship *curShip; //!< points to current ship, or nullptr if no ship selected

    int curX; //!< X coordinate of current position
    int curY; //!< Y coordinate of current position

    /**
     * @brief tryMove Tests to see new ship position is within bounds, if so, current values are changed.
     * @param newPiece used for horizontal or vertical orientation
     * @param newX new X coordinate to move to
     * @param newY new Y coordinate to move to
     * @return
     */
    bool tryMove(Ship *newPiece, int newX, int newY);


    QPushButton* curButton;//!< the current Ship button pressed, pressing cancel reshows this button

    /**
     * @brief focusShipButtons Disables cancel, confirm buttons when no ship is selected
     */
    void focusShipButtons();
    /**
     * @brief focusBoard Disables ship buttons, when ship is selected
     */
    void focusBoard();


    bool checkShips [6]={true,false,false,false,false,false}; //!<element true if ShipType is placed
    bool checkSetupDone();

    bool currentFocusShips = true; //!<true if currently focusing ships

    const int player; //!<player 1 or 2
    matrix *data = nullptr; //!<data to transfer
};

#endif // SETUPSCREEN_H
