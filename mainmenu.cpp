#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    auto layout = new QHBoxLayout(this);
    auto frame = new QFrame(this);

    QPixmap bkgnd(":/images/background.jpg");
    bkgnd = bkgnd.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette palette;palette.setBrush(QPalette::Background,bkgnd);
    this->setPalette(palette);

    frame->setLayout(layout);
    frame->setFixedSize(QSize(1000,690));
    frame->move(-14,0);
    frame->lower();
}

void MainMenu::on_buttonSingeplayer_clicked()
{
    emit clickedSingleplayer();
}

void MainMenu::on_buttonVersus_clicked()
{
    emit clickedVersus();
}

void MainMenu::on_buttonQuit_clicked()
{
    emit clickedQuit();
}

MainMenu::~MainMenu()
{
    delete ui;
}
