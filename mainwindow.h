#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "mainmenu.h"
#include "setupscreen.h"
#include "gamescreen.h"
#include "matrix.h"
#include "npc.h"
#include "dialog.h"
#include <QMediaPlayer>
#include "instructions.h"
#include <QTimer>

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
     * @brief resetAll Deletes current SetupScreen, GameScreen, NPC, allocate new instances
     */
    void resetAll();

    /**
     * @brief connectAll Reconnects SetupScreen, GameScreen, NPC after new instances
     */
    void connectAll();

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
    void switchGameScreen();

    /**
     * @brief toggleMusic If music is on -> turn off, vice versa.
     */
    void toggleMusic();

    /**
     * @brief showInstructions Shows instructions once at beginning.
     */
    void showInstructions();

private:
    Ui::MainWindow *ui;

    bool mute = false;

    void keyPressEvent(QKeyEvent *event);

    QStackedWidget *stackedWidget = nullptr;
    QFrame *firstscreen = nullptr;
    QFrame *singleplayerSetup = nullptr;
    QFrame *versusSetup1 = nullptr;
    QFrame *versusSetup2 = nullptr;
    QFrame *game = nullptr;
    NPC* bot = nullptr;
    Dialog* dialogGameover = nullptr;

    Instructions* instructions = nullptr;

    QMediaPlayer* music = nullptr;

};

#endif // MAINWINDOW_H
