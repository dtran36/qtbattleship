#include "gamescreen.h"
#include "ui_gamescreen.h"

GameScreen::GameScreen(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GameScreen)
{
    ui->setupUi(this);

    QPixmap textImage(":/Player1.png");
    ui->lbl_player1->setPixmap(textImage);

    srand(time(0)); // set random seed
    clearGrid();

    initialSize = destroyedShips.size();

    explosion = new QMediaPlayer();
    splash = new QMediaPlayer();
    explosion->setMedia(QUrl("qrc:/sounds/Explosion.mp3"));
    splash->setMedia(QUrl("qrc:/sounds/Splash.mp3"));
    //light up player1's name
    //this->show();
}

GameScreen::~GameScreen()
{
    delete ui;
}

void GameScreen::paintText(QPainter& painter, int x_coord, int y_coord)
{
    painter.translate(-3,188);

    const QString numbers[10]={"1","2","3","4","5","6","7","8","9","10"};
    const QString letters[10]={"A","B","C","D","E","F","G","H","I","J"};

    painter.setPen(Qt::gray);
    painter.setFont(QFont("Times New Roman", 16));

    // player 1's board
    for(int i = 0; i<10; i++)  // paints Player 1 grid letters
    {
        QRectF text(x_coord, y_coord, 30, 60);
        painter.drawText(text,Qt::AlignCenter, letters[i]);
        y_coord+=46;
    }
    y_coord += 13;
    x_coord = 23;
    for(int i = 0; i<10; i++)  // paints Player 1 grid numbers
    {
        QRectF text(x_coord, y_coord, 60, 20);
        painter.drawText(text,Qt::AlignCenter,numbers[i]);
        x_coord += 46;
    }

    // player 2's board
    painter.translate(500,0);
    x_coord = 5;
    y_coord = 5;
    for(int i = 0; i<10; i++)  // paints Player 2 grid letters
    {
        QRectF text(x_coord, y_coord, 30, 60);
        painter.drawText(text,Qt::AlignCenter, letters[i]);
        y_coord += 46;
    }
    y_coord += 13;
    x_coord = 23;
    for(int i = 0; i<10; i++)  // paints Player 2 grid numbers
    {
        QRectF text(x_coord, y_coord, 60, 20);
        painter.drawText(text,Qt::AlignCenter, numbers[i]);
        x_coord += 46;
    }
}

void GameScreen::checkIfDestroyed()
{
    // Player 2
    if (player2_carrier_hit == 5)
    {
        player2_carrier_destroyed = true;
    }
    if (player2_battleship_hit  == 4)
    {
        player2_battleship_destroyed = true;
    }
    if (player2_submarine_hit  == 4)
    {
        player2_submarine_destroyed = true;
    }
    if (player2_destroyer_hit  == 3)
    {
        player2_destroyer_destroyed = true;
    }
    if (player2_patrol_hit == 2)
    {
        player2_patrol_destroyed = true;
    }

    // Player 1
    if (player1_carrier_hit == 5)
    {
        player1_carrier_destroyed = true;
        destroyedShips.insert("carrier");
    }
    if (player1_battleship_hit  == 4)
    {
        player1_battleship_destroyed = true;
        destroyedShips.insert("battleship");
    }
    if (player1_submarine_hit  == 4)
    {
        player1_submarine_destroyed = true;
        destroyedShips.insert("submarine");
    }
    if (player1_destroyer_hit  == 3)
    {
        player1_destroyer_destroyed = true;
        destroyedShips.insert("destroyer");
    }
    if (player1_patrol_hit == 2)
    {
        player1_patrol_destroyed = true;
        destroyedShips.insert("patrol");
    }

    // if size increases, reset squares hit and successful second hit and continue direction
    if (is_computer)
    {
        // if size of QSet destroyedShips increases, reset computer info
        if (destroyedShips.size() > initialSize)
        {
            squaresHit = 0;
            successful_second_hit = false;
            continue_direction = 0;
            initialSize = destroyedShips.size();
        }
    }
}

void GameScreen::updateShips(QPainter& painter, int x_coord, int y_coord)
{
    painter.translate(-483, -130);
    //draw remaining ships for player 1
    // carrier
    for (int i = 0; i < 5; ++i)
    {
        if (!player1_carrier_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // battleship
    painter.translate(0,35);
    for (int i = 0; i < 4; ++i)
    {
        if (!player1_battleship_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // submarine
    painter.translate(0, 35);
    for (int i = 0; i < 4; ++i)
    {
        if (!player1_submarine_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // destroyer
    painter.translate(140,-70);
    for (int i = 0; i < 3; ++i)
    {
        if (!player1_destroyer_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // patrol boat
    painter.translate(0, 35);
    for (int i = 0; i < 2; ++i)
    {
        if (!player1_patrol_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }

    painter.translate(360, -35);
    //draw remaining ships for player 2
    // carrier
    for (int i = 0; i < 5; ++i)
    {
        if (!player2_carrier_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // battleship
    painter.translate(0, 35);
    for (int i = 0; i < 4; ++i)
    {
        if (!player2_battleship_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // submarine
    painter.translate(0, 35);
    for (int i = 0; i < 4; ++i)
    {
        if (!player2_submarine_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // destroyer
    painter.translate(140, -70);
    for (int i = 0; i < 3; ++i)
    {
        if (!player2_submarine_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
    // patrol boat
    painter.translate(0, 35);
    for (int i = 0; i < 2; ++i)
    {
        if (!player2_submarine_destroyed)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::red);
        }
        x_coord = 25*i;
        QRectF ship(x_coord, y_coord, 25, 25);
        painter.drawRect(ship);
    }
}

void GameScreen::paintBoard(QPainter& painter)
{
    painter.translate(-120, 107);
    //painter.translate(500, 0);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (player2Board[i][j] == blank)
            {
                continue;
            }
            else if (player2Board[i][j] == hit)
            {
                painter.setBrush(Qt::red);
                QRect attack(i*46, j*46, 46, 46);
                painter.drawRect(attack);
            }
            else if (player2Board[i][j] == miss)
            {
                painter.setBrush(Qt::blue);
                QRect miss(i*46, j*46, 46, 46);
                painter.drawRect(miss);
            }
        }
    }
    //qDebug() << count;
    //++count;
    painter.translate(-500,0);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (player1Board[i][j] == blank)
            {
                continue;
            }
            else if (player1Board[i][j] == hit)
            {
                painter.setBrush(Qt::red);
                QRect attack(i*46, j*46, 46, 46);
                painter.drawRect(attack);
            }
            else if (player1Board[i][j] == miss)
            {
                painter.setBrush(Qt::blue);
                QRect miss(i*46, j*46, 46, 46);
                painter.drawRect(miss);
            }
        }
    }
}

void GameScreen::gameplay(QPainter &painter)
{
    painter.translate(500, 0);
    if (!player1_turn)
    {
        // light up player name
        if (is_computer)  // Computer
        {
            if (squaresHit <= 0) // if no Hits
            {
                // choose random position on Player 1's grid
                rand_x_grid_pos = rand() % 10;
                rand_y_grid_pos = rand() % 10;
                origin_x = rand_x_grid_pos;
                origin_y = rand_y_grid_pos;
            }
            else if (squaresHit > 0 && successful_second_hit == false) // if one hit
            {
                rand_x_grid_pos = origin_x;
                rand_y_grid_pos = origin_y;

                rand_direction = 1 + rand() % 4; // choose random directions (1-4) from the hit square
                switch(rand_direction)
                {
                    case 1: // up
                        if (rand_y_grid_pos != 0)
                        {
                            --rand_y_grid_pos;
                        }
                        break;
                    case 2: // right
                        if (rand_x_grid_pos != 9)
                        {
                            ++rand_x_grid_pos;
                        }
                        break;
                    case 3: // down
                        if (rand_y_grid_pos != 9)
                        {
                            ++rand_y_grid_pos;
                        }
                        break;
                    case 4: // left
                        if (rand_x_grid_pos != 0)
                        {
                            --rand_x_grid_pos;
                        }
                        break;
                }
            }
            else if (squaresHit > 0 && successful_second_hit == true)  // if two or more hits
            {
                switch(continue_direction) // continue in the direction where the second hit was found
                {
                    case 1: // up
                        if (rand_y_grid_pos != 0)
                        {
                            --rand_y_grid_pos;
                        }
                        break;
                    case 2: // right
                        if (rand_x_grid_pos != 9)
                        {
                            ++rand_x_grid_pos;
                        }
                        break;
                    case 3: // down
                        if (rand_y_grid_pos != 9)
                        {
                            ++rand_y_grid_pos;
                        }
                        break;
                    case 4: // left
                        if (rand_x_grid_pos != 0)
                        {
                            --rand_x_grid_pos;
                        }
                        break;
                }
            }

            if (player1Ships[rand_x_grid_pos][rand_y_grid_pos] != empty) // if there is a ship
            {
                if (player1Board[rand_x_grid_pos][rand_y_grid_pos] == blank) // if coordinate has not been hit
                {
                    ++squaresHit;
                    if (squaresHit > 1 && successful_second_hit == false)  // hit second square
                    {
                        successful_second_hit = true;
                        continue_direction = rand_direction;  // continue with the current direction
                    }
                    player1Board[rand_x_grid_pos][rand_y_grid_pos] = hit;
                    updateHits(player1Ships[rand_x_grid_pos][rand_y_grid_pos]); // update ships destroyed counter
                    QTimer::singleShot(0, this, SLOT(soundUpdate())); // delay before next attack
                }
                else // if chosen coordinate has already been hit
                {
                    update();
                }
            }
            else // if there is no ship
            {
                if (player1Board[rand_x_grid_pos][rand_y_grid_pos] == blank) // if coordinate has not been missed
                {
                    if (squaresHit > 1) // if ship has not been destroyed completely
                    {
                        switch(continue_direction)  // continue in other direction
                        {
                            case 1:
                                continue_direction = 3;
                                break;
                            case 2:
                                continue_direction = 4;
                                break;
                            case 3:
                                continue_direction = 1;
                                break;
                            case 4:
                                continue_direction = 1;
                                break;
                        }
                    }
                    player1Board[rand_x_grid_pos][rand_y_grid_pos] = miss;
                    // turn off name
                    player1_turn = true; // Player 1's turn
                    QTimer::singleShot(0, this, SLOT(soundUpdate())); // delay before miss
                }
                else  // if chosen coordinate has already been missed
                {
                    update();
                }
            }
        }
        else // Player 2
        {
            if (click_x >= 32 && click_x <= 492 && click_y >= 200 && click_y <= 660) // Clicked coordinate is in correct area
            {
                // calculate correct grid position from mouse click
                int x_grid_pos = (click_x - 32) /46;
                int y_grid_pos = (click_y - 200) / 46;

                if (player1Ships[x_grid_pos][y_grid_pos] != empty) // if there is a ship
                {
                    if (player1Board[x_grid_pos][y_grid_pos] == blank) // if coordinate has not been hit
                    {
                        player1Board[x_grid_pos][y_grid_pos] = hit;
                        updateHits(player1Ships[x_grid_pos][y_grid_pos]); // update ships destroyed counter
                        explosion->play();
                        update();
                    }
                }
                else
                {
                    if (player1Board[x_grid_pos][y_grid_pos] == blank)
                    {
                        player1Board[x_grid_pos][y_grid_pos] = miss;
                        splash->play();
                        // turn off name
                        player1_turn = true;
                        update();
                    }
                }
            }
        }
    }

    painter.translate(-500, 0);
    if (player1_turn)
    {
        // light up player name
        if (click_x >= 532 && click_x <= 992 && click_y >= 200 && click_y <= 660)
        {
            int x_grid_pos = (click_x - 532) /46;
            int y_grid_pos = (click_y - 200) / 46;

            if (player2Ships[x_grid_pos][y_grid_pos] != empty)
            {
                if (player2Board[x_grid_pos][y_grid_pos] == blank)
                {
                    player2Board[x_grid_pos][y_grid_pos] = hit;
                    updateHits(player2Ships[x_grid_pos][y_grid_pos]);
                    explosion->play();
                    update();
                }
            }
            else
            {
                if (player2Board[x_grid_pos][y_grid_pos] == blank)
                {
                    player2Board[x_grid_pos][y_grid_pos] = miss;
                    splash->play();
                    // turn off name
                    player1_turn = false;
                    update();
                }
            }
        }
    }
}

void GameScreen::updateHits(ShipType hit_ship)
{
    if (player1_turn) // Player 1's turn
    {
        switch(hit_ship)
        {
        case empty:
            break;
        case carrier:
            ++player2_carrier_hit;
            break;
        case battleship:
            ++player2_battleship_hit;
            break;
        case submarine:
            ++player2_submarine_hit;
            break;
        case destroyer:
            ++player2_destroyer_hit;
            break;
        case patrol:
            ++player2_patrol_hit;
            break;
        }
    }
    if (!player1_turn) // Player 2's turn
    {
        switch(hit_ship)
        {
        case empty:
            break;
        case carrier:
            ++player1_carrier_hit;
            break;
        case battleship:
            ++player1_battleship_hit;
            break;
        case submarine:
            ++player1_submarine_hit;
            break;
        case destroyer:
            ++player1_destroyer_hit;
            break;
        case patrol:
            ++player1_patrol_hit;
            break;
        }
    }
}

void GameScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
   // qDebug() <<count;
   // ++count;
    int x_coord = 5;
    int y_coord = 5;
    paintText(painter, x_coord, y_coord); // paint grid text
    checkIfDestroyed();
    x_coord = 0;
    y_coord = 0;
    updateShips(painter, x_coord, y_coord); // paint remaining ships

    paintBoard(painter);
    gameplay(painter);

}

//void GameScreen::checkIfWinner()
//{
//    if (all of playerones ships are destroyed)
//    {
//        playertwo is winner;
//        repaint (show Player two wins);
//    }
//    else if (all of playertwos ships are destroyed)
//    {
//        playerone is winner;
//        repaint (show Player one wins);
//    }
//}

void GameScreen::mousePressEvent(QMouseEvent* event)
{
    click_x = event->x();
    click_y = event->y();
//    const QPointF point = event->windowPos();
//    qDebug() << point;

    if (is_computer)
    {
        if (!player1_turn)
        {
        }
        else {
            update();
        }
    }
    else
    {
        update();
    }
}

void GameScreen::clearGrid()
{
    for(int i = 0; i<10; ++i)
    {
        for(int j=0; j<10; ++j)
        {
            player1Board[i][j] = blank;
            player2Board[i][j] = blank;
            player2Ships[i][j] = empty;
            player1Ships[i][j] = empty;
        }
    }
    player1Ships[3][4] = carrier;
    player1Ships[3][5] = carrier;
    player1Ships[3][6] = carrier;
    player1Ships[3][7] = carrier;
    player1Ships[3][8] = carrier;

    player1Ships[5][4] = submarine;
    player1Ships[6][4] = submarine;
    player1Ships[7][4] = submarine;
    player1Ships[8][4] = submarine;

}

void GameScreen::soundUpdate()
{
    if (!player1_turn)
    {
        explosion->play();
    }
    else
    {
        splash->play();
    }
    update();
}

