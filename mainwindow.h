#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "mainmenu.h"
#include "setupscreen.h"
#include "gamescreen.h"
#include "matrix.h"
#include "singleplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief switchMainMenu Switches to Main Menu.
     */
    void switchMainMenu();

    /**
     * @brief switchSingleplayerSetup Switches to Setup.
     */
    void switchSingleplayerSetup();

    /**
     * @brief switchSetup1 Switches to Setup.
     */
    void switchSetup1();

    /**
     * @brief switchSetup2 Switches to Setup.
     */
    void switchSetup2();

    /**
     * @brief switchGameScreen Switches to Game Screen.
     */
    void switchVersusGame();

    void switchSingleplayerGame();
private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget = nullptr;
    QFrame *firstscreen = nullptr;
    QFrame *singleplayerSetup = nullptr;
    QFrame *versusSetup1 = nullptr;
    QFrame *versusSetup2 = nullptr;
    QFrame *versusGame = nullptr;
    QFrame *singleplayerGame = nullptr;
};

#endif // MAINWINDOW_H
