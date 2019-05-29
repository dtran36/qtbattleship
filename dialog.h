#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

signals:
    /**
     * @brief clk_Main Emitted when MainMenu button is clicked.
     */
    void clk_Main();
    /**
     * @brief clk_Quit Emitted when Quit button is clicked.
     */
    void clk_Quit();

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
    /**
     * @brief slot_hide Hides window.
     */
    void slot_hide();

    /**
     * @brief slot_show Shows window with specified winner.
     * @param X player that won, 1 or 2
     */
    void slot_show(const int X);

private slots:
    void on_btn_MainMenu_clicked();
    void on_btn_Quit_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
