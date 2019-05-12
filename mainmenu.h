#ifndef MAINMENU_H
#define MAINMENU_H

#include <QFrame>
#include <QPixmap>

namespace Ui {
class MainMenu;
}

class MainMenu : public QFrame
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    /**
     * @brief clickedSingleplayer Emitted when button is clicked.
     */
    void clickedSingleplayer();

    /**
     * @brief clickedVersus Emitted when button is clicked.
     */
    void clickedVersus();

    /**
     * @brief clickedQuit Emitted when button is clicked.
     */
    void clickedQuit();

private slots:
    void on_buttonSingeplayer_clicked();
    void on_buttonVersus_clicked();
    void on_buttonQuit_clicked();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
