#include "gamescreen.h"
#include "ui_gamescreen.h"

GameScreen::GameScreen(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GameScreen)
{
    ui->setupUi(this);
    explosion = new QSoundEffect(this);
    splash = new QSoundEffect(this);
    explosion->setSource(QUrl("qrc:/sounds/Explosion.wav"));
    splash->setSource(QUrl("qrc:/sounds/Splash.wav"));
    splash->setVolume(.6);

    //initialize hit or miss grids
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            player1HitorMiss[i][j]=unknown;
            player2HitorMiss[i][j]=unknown;
        }
    }
    pegRed = new QPixmap(":/pegs/pegRed.png");
    pegWhite = new QPixmap(":/pegs/pegWhite.png");
}

void GameScreen::setGrid(int player, const matrix& b)
{
    if(player==1)
        b.get_data(player2Grid);
    else //if(player==2)
        b.get_data(player1Grid);
}

void GameScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //board labels
    paintBoardLabels(painter);

    //current player turn
    if(currentPlayer==1)
    {
        ui->lbl_FingerLeft->show();
        ui->lbl_FingerRight->hide();
    }
    else {
        ui->lbl_FingerLeft->hide();
        ui->lbl_FingerRight->show();
    }

    const int w = 15;
    const int h = 40;

    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            //left board
            if(player1HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRect box((i*46)+33+21, (j*46)+200, w, h);

                painter.drawPixmap(box,*pegWhite);
            }
            if(player1HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::lightGray);
                QRect box((i*46)+38, (j*46)+200+36, 46, 10);
                painter.drawRect(box);

                painter.setBrush(Qt::red);
                QRect pegbox((i*46)+33+21, (j*46)+200, w, h);

                painter.drawPixmap(pegbox,*pegRed);
            }
            //right board
            if(player2HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRect box((i*46)+533+17, (j*46)+200, w, h);

                painter.drawPixmap(box,*pegWhite);
            }
            if(player2HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::lightGray);
                QRect box((i*46)+533, (j*46)+200+36, 46, 10);
                painter.drawRect(box);

                painter.setBrush(Qt::red);
                QRect pegbox((i*46)+533+17, (j*46)+200, w, h);

                painter.drawPixmap(pegbox,*pegRed);
            }
        }
    }
}

void GameScreen::paintBoardLabels(QPainter& painter)
{
    const QString numbers[10]={"1","2","3","4","5","6","7","8","9","10"};
    const QString letters[10]={"A","B","C","D","E","F","G","H","I","J"};
    painter.setPen(Qt::gray);
    painter.setFont(QFont("Times New Roman", 16));

    //left board labels
    int x = 0;
    int y = 200;
    for(int i = 0; i<10; i++)
    {
        QRectF test(x, y, 30, 50);
        painter.drawText(test,Qt::AlignCenter,numbers[i]);
        y+=46;
    }
    x+=30;
    for(int i = 0; i<10; i++)
    {
        QRectF test(x, y, 50, 20);
        painter.drawText(test,Qt::AlignCenter,letters[i]);
        x+=46;
    }
    //right board labels
    x = 500;
    y = 200;
    for(int i = 0; i<10; i++)
    {
        QRectF test(x, y, 30, 50);
        painter.drawText(test,Qt::AlignCenter,numbers[i]);
        y+=46;
    }
    x+=30;
    for(int i = 0; i<10; i++)
    {
        QRectF test(x, y, 50, 20);
        painter.drawText(test,Qt::AlignCenter,letters[i]);
        x+=46;
    }

    //draw grid lines
    for (int i = 0; i <= 10; ++i) {
        painter.drawLine(7,200+i*46, 1000,200+i*46);

        painter.drawLine(38+i*46,200,38+i*46,700);

        painter.drawLine(533+i*46,200,533+i*46,700);
    }

}

void GameScreen::mousePressEvent(QMouseEvent* event)
{
    click_x = event->x();
    click_y = event->y();

    if(currentPlayer==1)
    {
        if(click_x>500 || click_x<35 ||click_y<200 || click_y>660) return;
        int x_grid_pos = (click_x - 32) /46;
        int y_grid_pos = (click_y - 200) / 46;

        if(player1HitorMiss[x_grid_pos][y_grid_pos]!=unknown) return;

        if(player1Grid[x_grid_pos][y_grid_pos]==empty)
        {
            splash->play();
            player1HitorMiss[x_grid_pos][y_grid_pos]=miss;
        }
        else {
            explosion->play();
            player2Ships[player1Grid[x_grid_pos][y_grid_pos]]--;
            player1Grid[x_grid_pos][y_grid_pos]=empty;
            player1HitorMiss[x_grid_pos][y_grid_pos]=hit;
            checkIfDestroyed();
        }
        update();

        currentPlayer=2;
    }
    else //if (currentPlayer==2)
    {
        if(click_x<535 ||click_y<200 || click_y>660) return;
        int x_grid_pos = (click_x - 534) /46;
        int y_grid_pos = (click_y - 202) / 46;

        if(player2HitorMiss[x_grid_pos][y_grid_pos]!=unknown) return;

        if(player2Grid[x_grid_pos][y_grid_pos]==empty)
        {
            splash->play();
            player2HitorMiss[x_grid_pos][y_grid_pos]=miss;
        }
        else {
            explosion->play();
            player2Ships[player2Grid[x_grid_pos][y_grid_pos]]--;
            player2Grid[x_grid_pos][y_grid_pos]=empty;
            player2HitorMiss[x_grid_pos][y_grid_pos]=hit;
            checkIfDestroyed();
        }
        currentPlayer=1;
        update();
    }
}

void GameScreen::checkIfDestroyed()
{
    lastShotSunk = false;

    bool oldShipsSunk[6];

    for (int i = 1; i < 6; ++i)
        oldShipsSunk[i] = player1ShipsSunk[i];

    if(player1Ships[carrier]==0) {ui->carrierLeft->hide(); player1ShipsSunk[carrier] = true;}
    if(player1Ships[battleship]==0) {ui->battleshipLeft->hide(); player1ShipsSunk[battleship] = true;}
    if(player1Ships[submarine]==0) {ui->submarineLeft->hide(); player1ShipsSunk[submarine] = true;}
    if(player1Ships[destroyer]==0) {ui->destroyerLeft->hide(); player1ShipsSunk[destroyer] = true;}
    if(player1Ships[patrol]==0) {ui->patrolLeft->hide(); player1ShipsSunk[patrol] = true;}

    for (int i = 1; i < 6; ++i)
        if(oldShipsSunk[i] != player1ShipsSunk[i])
            lastShotSunk = true;

    if(player2Ships[carrier]==0) ui->carrierRight->hide();
    if(player2Ships[battleship]==0) ui->battleshipRight->hide();
    if(player2Ships[submarine]==0) ui->submarineRight->hide();
    if(player2Ships[destroyer]==0) ui->destroyerRight->hide();
    if(player2Ships[patrol]==0) ui->patrolRight->hide();

    bool winPlayer1=true;
    bool winPlayer2=true;
    for(int i=0; i<6;i++)
    {
        if(player2Ships[i]!=0)
            winPlayer1=false;
        if(player1Ships[i]!=0)
            winPlayer2=false;
    }
    if(winPlayer1){playerXWins(1);}
    if(winPlayer2){playerXWins(2);}
}

void GameScreen::playerXWins(const int x)
{
    if(x==1)
    {
        ui->lbl_Player2->hide();
    }
    else {
        ui->lbl_Player1->hide();
    }
}

GameScreen::~GameScreen()
{
    delete ui;
}
