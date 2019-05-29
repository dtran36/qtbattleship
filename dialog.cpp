#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setModal(true);

    move(250,250);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btn_MainMenu_clicked()
{
    slot_hide();
    emit clk_Main();
}

void Dialog::on_btn_Quit_clicked()
{
    slot_hide();
    emit clk_Quit();
}

void Dialog::slot_hide()
{
    this->hide();
}

void Dialog::slot_show(const int X)
{
    if(X==1)
    {
        ui->label->setPixmap(QPixmap(":/images/player1wins.png"));
    }
    else if(X==2)
    {
        ui->label->setPixmap(QPixmap(":/images/player2wins.png"));
    }
    else {
        ui->label->setPixmap(QPixmap(":/images/paused.png"));
    }
    this->show();
}
