#include "singleplayer.h"

singleplayer::singleplayer()
{
    //initialize prob grid
    const int ref[100] = {
        23,35,44,49,51,51,49,44,35,23,
        35,46,55,60,62,62,60,55,46,35,
        44,55,65,69,71,71,69,65,55,44,
        49,60,69,74,76,76,74,69,60,49,
        51,62,71,76,78,78,76,71,62,51,
        51,62,71,76,78,78,76,71,62,51,
        49,60,69,74,76,76,74,69,60,49,
        44,55,65,69,71,71,69,65,55,44,
        35,46,55,60,62,62,60,55,46,35,
        23,35,44,49,51,51,49,44,35,23
    };

    int counter = 0;
    //initialize hit or miss grids
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            probGrid[i][j] = ref[i];
            counter++;
        }
    }

    //randomize player2 grid
    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            player2Grid[i][j]=empty;
        }
    }

    randomize_board();

    //visuals and rand
    currentPlayer=1;
    srand(time(0));
}

void singleplayer::mousePressEvent(QMouseEvent* event)
{
    click_x = event->x();
    click_y = event->y();

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

    x_grid_pos = shot.first;
    y_grid_pos = shot.second;

    if(player2HitorMiss[x_grid_pos][y_grid_pos]!=unknown)
    {
        qDebug()<<"NOTE: FIRING AT KNOWN POSITION!";

        std::vector<std::pair<int,std::pair<int,int>>> tempBuffer;
        const int arrX[8] = {1,-1,0,0,-1,1,-1,1};
        const int arrY[8] = {0,0,-1,1,-1,1,1,-1};

        for (int i = 0; i < 8; ++i)
        {
            int x = origSquare.first+arrX[i];
            int y = origSquare.second+arrY[i];
            if (x < 0 || x >= 10 || y < 0 || y >= 10 || probGrid[x][y]==0)
            {
                continue;
            }
            tempBuffer.push_back(std::make_pair(probGrid[x][y],std::make_pair(x,y)));
        }
        targetMode = false;
        if (tempBuffer.empty())
        {
            qDebug()<<"ERROR: EMPTY TEMP BUFFER";
        }
        else {
            shot = tempBuffer[tempBuffer.size()-1].second;
        }
    }

    lastX = x_grid_pos;
    lastY = y_grid_pos;

//NPC MISS
    if(player2Grid[x_grid_pos][y_grid_pos]==empty)
    {
        splash->play();
        player2HitorMiss[x_grid_pos][y_grid_pos]=miss;

        if(targetMode)
        {
            if (currDirection!=none)
            {
                reverseCounter++;
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

//NPC HIT
    else
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
    update();
}

bool singleplayer::placeRandom(Ship *newPiece, int newX, int newY)
{
    for (int i = 0; i < 5; ++i)
    {
        int x = newX + newPiece->x(i);
        int y = newY - newPiece->y(i);

        // if X or Y is not within 0-9, or square is taken
        if (x < 0 || x >= 10 || y < 0 || y >= 10 || player2Grid[x][y]!=empty)
            return false; //new position ship is out of bounds

        // strategic placement: no ships are touching
        int arrX[4] = {x+1,x-1,x,x};
        int arrY[4] = {y,y,y+1,y-1};
        for (int j =0;j<4;j++) {
            if (player2Grid[arrX[j]][arrY[j]]!=newPiece->getType() && player2Grid[arrX[j]][arrY[j]]!=empty)
                return false;
        }
    }

    // new position ship is within grid bounds
    curX = newX; // set new curX
    curY = newY; // set new curY
    return true;
}

void singleplayer::randomize_board()
{
    //randomize orientation
    bool horizontal[6] = {}; // true for horizontal
    for (int i =0;i<6;i++)
    {
        horizontal[i] = rand() % 2;
    }
    const ShipType refarr[6] = {empty,carrier,battleship,submarine,destroyer,patrol};

    //randomize placement
    for (int i =1; i<6;i++) // skip first element(empty ShipType)
    {
        curShip = new Ship(refarr[i]);
        if(horizontal[i])
            curShip->rotate();

        bool placed = false;
        int randX = 0;
        int randY = 0;
        while(!placed)
        {
            randX = rand() % 10;
            randY = rand() % 10;
            placed=placeRandom(curShip,randX, randY);
        }

        for(int i =0; i<5; i++)
        {
            player2Grid[curX + curShip->x(i)][curY - curShip->y(i)]=curShip->getType();
        }
        delete curShip;
    }
}

void singleplayer::adjustProbGrid(std::pair<int,int> input)
{
    const int x = input.first; //save x value
    const int y = input.second; //save y value

    const int effectSize = 3; // number adjacent squares to adjust

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
                    if (x_dir+1 >=10 || probGrid[x_dir+1][y]==0)
                        break;
                    if(player2HitorMiss[x][y]==miss)
                        probGrid[++x_dir][y]-=minusprob;
                    else
                        probGrid[++x_dir][y]+=minusprob;
                    minusprob-=5;
                }
                break;
            case 1: // adjust left
                for (int j=0; j<effectSize;j++)
                {
                    if (x_dir-1 < 0 || probGrid[x_dir-1][y]==0)
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
                    if (y_dir+1 >=10 || probGrid[x][y_dir+1]==0)
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
                    if (y_dir-1 < 0 || probGrid[x][y_dir-1]==0)
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

std::pair<int,int> singleplayer::generateSearchShot()
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

            // if (probGrid[i][j]>max)
            else
            {
                max = probGrid[i][j];
                possibleShots.clear();
                possibleShots.push_back(std::make_pair(i, j));
            }
        }
    }
    return possibleShots[rand() % possibleShots.size()];
}

std::pair<int,int> singleplayer::generateTargetShot()
{
    std::pair<int,int> rval = std::make_pair(0,0);

    if(lastShotMissed && secondLastShotMissed)
    {
        orientation=0;
        currDirection = none;
    }

    if (orientation==0)
    {
        if(turnBuffer.empty())
        {
//            qDebug()<<"Orientation is currently unknown.";

            //Search the 4 adjacent squares for highest probability.
            const int arrX[4] = {1,-1,0,0};
            const int arrY[4] = {0,0,-1,1};

            //right,left,up,down
            for (int i = 0; i < 4; ++i)
            {
                int x = origSquare.first+arrX[i];
                int y = origSquare.second+arrY[i];
                if (x < 0 || x >= 10 || y < 0 || y >= 10)
                {
                    continue;
                }
                turnBuffer.push_back(std::make_pair(probGrid[x][y],std::make_pair(x,y)));
            }
            std::sort(turnBuffer.begin(),turnBuffer.end());

//            for (int i = 0; i < 4; ++i) {
//                qDebug()<<turnBuffer[i].second.first<<","<<turnBuffer[i].second.second;
//            }
        }
        rval = turnBuffer[turnBuffer.size()-1].second;
        turnBuffer.pop_back();
    }
    else //if(orientation!=0)//ORIENTATION FOUND
    {
        qDebug()<<"ORIENTATION FOUND";

        bool done = false;

        while(!done)
        {
            switch (currDirection) {
            case none:
                qDebug()<<"ERROR: NO DIRECTION";
                break;
            case up:
                qDebug()<<"UP from LAST SHOT"<<lastX<<lastY;

                rval = std::make_pair(lastX,lastY-1);

                while (player2HitorMiss[rval.first][rval.second]==hit)
                {
                    qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, UP AGAIN";
                    rval.second-=1;
                }

                if(player2HitorMiss[rval.first][rval.second]==miss)
                {
                    qDebug()<<"UP AGAIN ENCOUNTERED MISS";
                    reverseCounter++;
                    if (reverseCounter == 2)
                    {
                        qDebug()<<"REVERSE COUNTER ==2";
                        orientation = 0;
                        reverseCounter = 0;
                        return generateTargetShot();
                    }
                    reverseDirection();
                    lastX = origSquare.first;
                    lastY = origSquare.second;
                    rval = std::make_pair(lastX,lastY+1);
                }

                break;
            case down:
                qDebug()<<"DOWN from LAST SHOT"<<lastX<<lastY;

                rval = std::make_pair(lastX,lastY+1);

                while (player2HitorMiss[rval.first][rval.second]==hit)
                {
                    qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, DOWN AGAIN";
                    rval.second+=1;
                }

                if(player2HitorMiss[rval.first][rval.second]==miss)
                {
                    qDebug()<<"DOWN AGAIN ENCOUNTERED MISS";
                    reverseCounter++;
                    if (reverseCounter == 2)
                    {
                        qDebug()<<"REVERSE COUNTER ==2";
                        orientation = 0;
                        reverseCounter = 0;
                        return generateTargetShot();
                    }
                    reverseDirection();
                    lastX = origSquare.first;
                    lastY = origSquare.second;
                    rval = std::make_pair(lastX,lastY-1);
                }

                break;
            case left_:
                qDebug()<<"LEFT from LAST SHOT"<<lastX<<lastY;

                rval = std::make_pair(lastX-1,lastY);

                while (player2HitorMiss[rval.first][rval.second]==hit)
                {
                    qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, LEFT AGAIN";
                    rval.first-=1;
                }

                if(player2HitorMiss[rval.first][rval.second]==miss)
                {
                    qDebug()<<"LEFT AGAIN ENCOUNTERED MISS";
                    reverseCounter++;
                    if (reverseCounter == 2)
                    {
                        qDebug()<<"REVERSE COUNTER ==2";
                        orientation = 0;
                        reverseCounter = 0;
                        return generateTargetShot();
                    }
                    reverseDirection();
                    lastX = origSquare.first;
                    lastY = origSquare.second;
                    rval = std::make_pair(lastX+1,lastY);
                }

                break;
            case right_:
                qDebug()<<"RIGHT from LAST SHOT:"<<lastX<<lastY;

                rval = std::make_pair(lastX+1,lastY);

                while (player2HitorMiss[rval.first][rval.second]==hit)
                {
                    qDebug()<<"INTEDEND SHOT SQUARE ALREADY HIT, RIGHT AGAIN";
                    rval.first+=1;
                }

                if(player2HitorMiss[rval.first][rval.second]==miss)
                {
                    qDebug()<<"RIGHT AGAIN ENCOUNTERED MISS";
                    reverseCounter++;
                    if (reverseCounter == 2)
                    {
                        qDebug()<<"REVERSE COUNTER ==2";
                        orientation = 0;
                        reverseCounter = 0;
                        return generateTargetShot();
                    }
                    reverseDirection();
                    lastX = origSquare.first;
                    lastY = origSquare.second;
                    rval = std::make_pair(lastX-1,lastY);
                }

                break;
            }
            if (rval.first < 0 || rval.first >= 10 || rval.second < 0 || rval.second >= 10)
            {
                reverseCounter++;
                if (reverseCounter == 2)
                {
                    qDebug()<<"REVERSE COUNTER ==2";
                    orientation = 0;
                    reverseCounter = 0;
                    return generateTargetShot();
                }
                else {
                    reverseDirection();
                    lastX = origSquare.first;
                    lastY = origSquare.second;
                }
            }
            else {
                done = true;
            }
        }
    }

    lastX = rval.first;
    lastY = rval.second;

    return rval;
}

void singleplayer::setOrigSquare(std::pair<int,int> newFirstHit)
{
    qDebug()<<"SETTING NEW OG SQUARE";
    origSquare = newFirstHit;
    orientation = 0;
    currDirection = none;
}

void singleplayer::reverseDirection()
{
    switch (currDirection) {
    case none:
        qDebug()<<"ERROR: NO DIRECTION TO REVERSE";
        break;
    case up:
        qDebug()<<"SWITCHED DIRECTION U->D";
        currDirection = down;
        break;
    case down:
        qDebug()<<"SWITCHED DIRECTION D->U";
        currDirection = up;
        break;
    case left_:
        qDebug()<<"SWITCHED DIRECTION L->R";
        currDirection = right_;
        break;
    case right_:
        qDebug()<<"SWITCHED DIRECTION R->L";
        currDirection = left_;
        break;
    }
}
