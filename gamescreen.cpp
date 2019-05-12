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

    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            player1HitorMiss[i][j]=unknown;
            player2HitorMiss[i][j]=unknown;
        }
    }
}

void GameScreen::setGrid(int player, const matrix& b)
{
    if(player==1)
        b.get_data(player1Grid);
    else //if(player==2)
        b.get_data(player2Grid);
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

    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            //left board
            if(player1HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRectF test((i*46)+33, (j*46)+200, 46, 46);
                painter.drawRect(test);
            }
            if(player1HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::red);
                QRectF test((i*46)+33, (j*46)+200, 46, 46);
                painter.drawRect(test);
            }

            //right board
            if(player2HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRectF test((i*46)+533, (j*46)+200, 46, 46);
                painter.drawRect(test);
            }
            if(player2HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::red);
                QRectF test((i*46)+533, (j*46)+200, 46, 46);
                painter.drawRect(test);
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
            player1Ships[player1Grid[x_grid_pos][y_grid_pos]]--;
            player1Grid[x_grid_pos][y_grid_pos]=empty;
            player1HitorMiss[x_grid_pos][y_grid_pos]=hit;
            checkIfDestroyed();
        }

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
    }
    update();
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
    for(int i=0; i<6;i++)
    {
        if(player1Ships[i]!=0)
            winPlayer1=false;
        if(player2Ships[i]!=0)
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
