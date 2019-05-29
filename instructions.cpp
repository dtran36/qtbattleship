#include "instructions.h"
#include "ui_instructions.h"

Instructions::Instructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instructions)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    move(250,250);
}

Instructions::~Instructions()
{
    delete ui;
}

void Instructions::on_pushButton_clicked()
{
    this->hide();
}
