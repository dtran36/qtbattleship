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
    void clk_Main();
    void clk_Quit();

public slots:
    void slot_hide();
    void slot_show(const int X);

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btn_MainMenu_clicked();
    void on_btn_Quit_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
