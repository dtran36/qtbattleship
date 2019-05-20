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

            probGrid[i][j]=100;
        }
    }

    leftPegMiss = new QPixmap(":/pegs/leftPegMiss.png");
    leftPegHit = new QPixmap(":/pegs/leftPegHit.png");
    rightPegMiss = new QPixmap(":/pegs/rightPegMiss.png");
    rightPegHit = new QPixmap(":/pegs/rightPegHit.png");
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

    const int w = 60;
    const int h = 60;

    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            //left board
            if(player1HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRect box((i*46)+33-24, (j*46)+200-24, w, h);

                painter.drawPixmap(box,*leftPegMiss);
            }
            if(player1HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::red);
                QRect box((i*46)+33-24, (j*46)+200-24, w, h);

                painter.drawPixmap(box,*leftPegHit);
            }

            //right board
            if(player2HitorMiss[i][j]==miss)
            {
                painter.setBrush(Qt::yellow);
                QRect box((i*46)+533+20, (j*46)+200-24, w, h);

                painter.drawPixmap(box,*rightPegMiss);
            }
            if(player2HitorMiss[i][j]==hit)
            {
                painter.setBrush(Qt::red);
                QRect box((i*46)+533+20, (j*46)+200-24, w, h);

                painter.drawPixmap(box,*rightPegHit);
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
//    painter.drawLine(10,10, 300,300);
    for (int i = 0; i <= 10; ++i) {
        painter.drawLine(7,200+i*46, 500,200+i*46);

        painter.drawLine(7,240,500,500);
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

        if (versus)
        {
            currentPlayer=2;
        }
        else //NPC TURN
        {
            std::pair<int,int> shot;
            if (targetMode)
            {
                qDebug()<<"GENERATING TARGET SHOT";
                shot = generateTargetShot();
            }
            else {
                qDebug()<<"GENERATING NORMAL SHOT";
                shot = generateSearchShot();
            }

            int x_grid_pos = shot.first;
            int y_grid_pos = shot.second;

            if(player2HitorMiss[x_grid_pos][y_grid_pos]!=unknown)
            {
                qDebug()<<"NOTE: FIRING AT KNOWN POSITION!";
            }

            lastX = x_grid_pos;
            lastY = y_grid_pos;

            if(player2Grid[x_grid_pos][y_grid_pos]==empty) //NPC MISS
            {
                splash->play();
                player2HitorMiss[x_grid_pos][y_grid_pos]=miss;

                if(targetMode) //reverse direction
                {
                    if (currDirection!=none)
                    {
                        reverseDirection();
                        lastX = origSquare.first;
                        lastY = origSquare.second;
                    }
                }
                adjustProbGrid(shot);
                lastShotSunk = false;

                secondLastShotMissed = lastShotMissed;
                lastShotMissed = true;
            }
            else //NPC HIT
            {
                explosion->play();
                player1Ships[player2Grid[x_grid_pos][y_grid_pos]]--;
                player2Grid[x_grid_pos][y_grid_pos]=empty;
                player2HitorMiss[x_grid_pos][y_grid_pos]=hit;
                checkIfDestroyed();

                secondLastShotMissed = lastShotMissed;
                lastShotMissed = false;

                if (targetMode)
                {
                    if(lastShotSunk)
                    {
                        qDebug()<<"SWITCHING TO SEARCHING MODE";
                        targetMode = false;
                        lastShotSunk = false;
                    }
                    else{
                        if(orientation==0)
                        {
                            if(shot.first == origSquare.first) // X did not change = vertical orientation
                            {
                                orientation = 2;
                                int diffY = shot.second-origSquare.second;
                                if(diffY > 0)
                                {
                                    currDirection = down;
                                }
                                else {
                                    currDirection = up;
                                }
                            }
                            if(shot.second == origSquare.second) // Y did not change = horizontal orientation
                            {
                                orientation = 1;
                                int diffX = shot.first-origSquare.first;
                                if(diffX > 0)
                                {
                                    currDirection = right_;
                                }
                                else {
                                    currDirection = left_;
                                }
                            }
    //                        qDebug()<<"ORIENTATION IS:"<<orientation;
                            qDebug()<<"DIRECTION IS:"<<currDirection;
                        }
                    }
                }
                else
                {
                    qDebug()<<"NEW FIRST HIT:("<<origSquare.first<<","<<origSquare.second<<")";
                    if(lastShotSunk)
                    {
                        qDebug()<<"FIRST HIT SUNK, SWITCHING TO SEARCHING MODE";
                        targetMode = false;
                        lastShotSunk = false;
                    }
                    else {
                        qDebug()<<"SWITCHING TO TARGETING MODE";
                        targetMode = true;
                        setOrigSquare(shot);
                        lastX = shot.first;
                        lastY = shot.second;
                        qDebug()<<"CLEARING TURN BUFFER";
                        turnBuffer.clear();
                    }
                }
                adjustProbGrid(shot);
            }
            currentPlayer=1;
            update();

//            qDebug()<<"LAST SHOT:("<<lastShot.first<<","<<lastShot.second<<")";
//            qDebug()<<"TARGET MODE:"<<targetMode;
//            qDebug()<<"LAST SHOT SUNK:"<< lastShotSunk;
        }
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

void GameScreen::setVersus()
{
    versus = true;
}

std::pair<int,int> GameScreen::generateSearchShot()
{
    int max = 0;
    std::vector<std::pair<int,int>> possibleShots;
    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            if (probGrid[i][j]<max)
                continue;
            else if (probGrid[i][j]==max) possibleShots.push_back(std::make_pair(i, j));
            else // if (probGrid[i][j]>max)
            {
                max = probGrid[i][j];
                possibleShots.clear();
                possibleShots.push_back(std::make_pair(i, j));
            }
        }
    }
    return possibleShots[rand() % possibleShots.size()];
}


void GameScreen::adjustProbGrid(std::pair<int,int> input)
{
    const int x = input.first; //save x value
    const int y = input.second; //save y value

    const int effectSize = 2; // number adjacent squares to adjust

    probGrid[x][y]=0; //set center to 0 probability

    for (int i =0;i<4;i++) // adjust probability of 4 adjacent squares
    {
        int x_dir = x;
        int y_dir = y;

        int minusprob = effectSize*5;
        switch (i) {
            case 0: // adjust right
                for (int j=0; j<effectSize;j++)
                {
                    if (x_dir+1 >=10)
                        break;
                    if(player2HitorMiss[x][y]==miss)
                        probGrid[++x_dir][y]-=minusprob;
                    else {
                        probGrid[++x_dir][y]+=minusprob;
                    }
                    minusprob-=5;
                }
                break;
            case 1: // adjust left
                for (int j=0; j<effectSize;j++)
                {
                    if (x_dir-1 < 0)
                        break;
                    if(player2HitorMiss[x][y]==miss)
                        probGrid[--x_dir][y]-=minusprob;
                    else {
                        probGrid[--x_dir][y]+=minusprob;
                    }
                    minusprob-=5;
                }
                break;

            case 2: // adjust down
                for (int j=0; j<effectSize;j++)
                {
                    if (y_dir+1 >=10)
                        break;
                    if(player2HitorMiss[x][y]==miss)
                        probGrid[x][++y_dir]-=minusprob;
                    else {
                        probGrid[x][++y_dir]+=minusprob;
                    }
                    minusprob-=5;
                }
                break;

            case 3: //adjust up
                for (int j=0; j<effectSize;j++)
                {
                    if (y_dir-1 < 0)
                        break;
                    if(player2HitorMiss[x][y]==miss)
                        probGrid[x][--y_dir]-=minusprob;
                    else {
                        probGrid[x][--y_dir]+=minusprob;
                    }
                    minusprob-=5;
                }
                break;
            default:
                break;
        }
    }
}

GameScreen::~GameScreen()
{
    delete ui;
}
