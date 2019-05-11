#include "gamescreen.h"
#include "ui_gamescreen.h"

#include<QDebug>

GameScreen::GameScreen(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GameScreen)
{
    ui->setupUi(this);
//    explosion = new QMediaPlayer();
//    splash = new QMediaPlayer();
//    explosion->setMedia(QUrl("qrc:/sounds/Explosion.mp3"));
//    splash->setMedia(QUrl("qrc:/sounds/Splash.mp3"));

    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            player1HitorMiss[i][j]=unknown;
        }
    }

//    for(int i=0; i<10; i++)
//    {
//        for(int j=0; j<10; j++)
//        {
//            player1Grid[i][j]=empty;
//            player2Grid[i][j]=empty;
//        }
//    }
}

GameScreen::~GameScreen()
{
    delete ui;
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
}

void GameScreen::checkIfDestroyed()
{
    if(player1Ships[carrier]==0) ui->carrierLeft->hide();
    if(player1Ships[battleship]==0) ui->battleshipLeft->hide();
    if(player1Ships[submarine]==0) ui->submarineLeft->hide();
    if(player1Ships[destroyer]==0) ui->destroyerLeft->hide();
    if(player1Ships[patrol]==0) ui->patrolLeft->hide();

    if(player2Ships[carrier]==0) ui->carrierRight->hide();
    if(player2Ships[battleship]==0) ui->battleshipRight->hide();
    if(player2Ships[submarine]==0) ui->submarineRight->hide();
    if(player2Ships[destroyer]==0)ui->destroyerRight->hide();
    if(player2Ships[patrol]==0) ui->patrolRight->hide();

    bool winPlayer1=true;
    bool winPlayer2=true;
    for(int i=0; i<5;i++)
    {
        if(player1Ships[i]!=0)
            winPlayer1=false;
        if(player2Ships[i]!=0)
            winPlayer2=false;
    }
//    if(winPlayer1){playerXWins(1);}
//    if(winPlayer2){playerXWins(2);}
}

void GameScreen::updateShips(QPainter& painter, int x_coord, int y_coord){}

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

    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(player1Grid[i][j]!=empty)
            {
                painter.setBrush(Qt::darkGray);
                QRectF test((i*46)+33, (j*46)+200, 46, 46); // box height & width = 60
                painter.drawRect(test);
            }
        }
    }
}

void GameScreen::mousePressEvent(QMouseEvent* event)
{
    click_x = event->x();
    click_y = event->y();

//    ui->lcd_left->display(x_grid_pos);
//    ui->lcd_right->display(y_grid_pos);

    if(currentPlayer==1)
    {
//        if(click_x>500 || click_y<200) return;
//        int x_grid_pos = (click_x - 32) /46;
//        int y_grid_pos = (click_y - 200) / 46;

//        if(player1HitorMiss[x_grid_pos][y_grid_pos]!=unknown) return;

//        else {
//            if(player1Grid[x_grid_pos][y_grid_pos]==empty)
//            {
//                player1HitorMiss[x_grid_pos][y_grid_pos]=miss;
//            }
//            else {
//                player1Ships[player1Grid[x_grid_pos][y_grid_pos]]--;
//                player1Grid[x_grid_pos][y_grid_pos]=empty;
//                player1HitorMiss[x_grid_pos][y_grid_pos] = hit;
//                checkIfDestroyed();
//            }
//        }
        currentPlayer=2;
    }
    else //if (currentPlayer==2)
    {
        if(click_x<500 || click_y<200) return;
        int x_grid_pos = (click_x - 534) /46;
        int y_grid_pos = (click_y - 202) / 46;


//        if(player2HitorMiss[x_grid_pos][y_grid_pos]!=unknown) return;

//        else {
//            if(player2Grid[x_grid_pos][y_grid_pos]==empty)
//            {
//                player2HitorMiss[x_grid_pos][y_grid_pos]=miss;
//            }
//            else {
//                player2Ships[player2Grid[x_grid_pos][y_grid_pos]]--;
//                player2Grid[x_grid_pos][y_grid_pos]=empty;
//                player2HitorMiss[x_grid_pos][y_grid_pos] = hit;
//                checkIfDestroyed();
//            }
//        }
        currentPlayer=1;
    }
    update();
}


void GameScreen::setGrid(int player, ShipType arr[10][10])
{
    if(player==1)
    {
        for(int i =0; i<10; i++) {
            for(int j =0; j<10; j++)
                player1Grid[i][j] = arr[i][j];}

        for(int i=0; i<10; i++)
        {
            for(int j=0; j<10; j++)
            {
                qDebug()<<arr[i][j]<<" ";
            }
            qDebug()<<endl;
        }
    }
    else //if(player==2)
    {
        for(int i =0; i<10; i++) {
            for(int j =0; j<10; j++)
                player2Grid[i][j] = arr[i][j];}
    }
}
