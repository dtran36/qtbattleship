#include "setupscreen.h"
#include "ui_setupscreen.h"

SetupScreen::SetupScreen(int x,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SetupScreen),
    player(x)
{
    ui->setupUi(this);

    if(x==1) //player 1 specified
    {
        QPixmap textImage(":/images/setupPlayer1.png");
        ui->lblSetupTitle->setPixmap(textImage);
        setWindowTitle("Setup Player 1");
    }
    else //default is player 2
    {
        QPixmap textImage(":/images/setupPlayer2.png");
        ui->lblSetupTitle->setPixmap(textImage);
        setWindowTitle("Setup Player 2");
    }

    //set current position so Carrier and Battleship are not offscreen
    curX=0;
    curY=2;

    //resets the board
    clearGrid();
    repaint();

    //initialize curShip with empty Ship
    curShip = new Ship(empty);

    //initialize curButton
    curButton = nullptr;

    //sets focus on the window
    setFocusPolicy(Qt::StrongFocus);

    //make sure user can't press confirm, cancel before selecting first ship
    ui-> pushButton -> setEnabled(false);
    ui-> pushButton_2 -> setEnabled(false);

    ui->Warning->hide();

    ui->pushCarrier->setStyleSheet("border: none;");
    ui->pushBattleship->setStyleSheet("border: none;");
    ui->pushSubmarine->setStyleSheet("border: none;");
    ui->pushDestroyer->setStyleSheet("border: none;");
    ui->pushPatrol->setStyleSheet("border: none;");
}

void SetupScreen::on_pushCarrier_clicked()
{
    //hides current button after it is pressed
    curButton = ui->pushCarrier;
    curButton->hide();

    //delete the previous curShip
    delete curShip;

    //set curShip to heap object of the type specified
    curShip = new Ship(carrier);

    //shows the curShip
    repaint();

    //sets position of warning to current ship
    ui->Warning->setGeometry(75,142,200,40);

    //make sure user can't press ship buttons while curShip is not empty
    focusBoard();
}

void SetupScreen::on_pushBattleship_clicked()
{
    curButton = ui->pushBattleship;
    curButton->hide();
    delete curShip;
    curShip = new Ship(battleship);
    repaint();
    focusBoard();
    ui->Warning->setGeometry(75,262,200,40);
}

void SetupScreen::on_pushSubmarine_clicked()
{
    curButton = ui->pushSubmarine;
    curButton->hide();
    delete curShip;
    curShip = new Ship(submarine);
    repaint();
    focusBoard();
    ui->Warning->setGeometry(75,382,200,40);
}

void SetupScreen::on_pushDestroyer_clicked()
{
    curButton = ui->pushDestroyer;
    curButton->hide();
    delete curShip;
    curShip = new Ship(destroyer);
    repaint();
    focusBoard();
    ui->Warning->setGeometry(75,502,200,40);
}

void SetupScreen::on_pushPatrol_clicked()
{
    curButton = ui->pushPatrol;
    curButton->hide();
    delete curShip;
    curShip = new Ship(patrol);
    repaint();
    focusBoard();
    ui->Warning->setGeometry(75,624,200,40);
}

void SetupScreen::on_pushButton_clicked() //user clicks "confirm"
{
    confirmPlacement();
}

void SetupScreen::paintEvent(QPaintEvent* event)
{

    //paint grid letters and numbers
    QPainter painter(this); // create painter object
    painter.translate(340,0); //move painter position to top left of grid
    int x = 0;
    int y = 0;
    const QString arr[10]={"10","9","8","7","6","5","4","3","2","1"};
    const QString arr2[10]={"A","B","C","D","E","F","G","H","I","J"};
    painter.setPen(Qt::gray);
    painter.setFont(QFont("Times New Roman", 20));

    //paint numbers
    for(int i = 0; i<10; i++)
    {
        QRectF rectNumber(x, y, 30, 60);
        painter.drawText(rectNumber,Qt::AlignCenter,arr[i]);
        y+=60;
    }
    x=30;

    //paint letters
    for(int i = 0; i<10; i++)
    {
        QRectF rectLetter(x, y, 60, 30);
        painter.drawText(rectLetter,Qt::AlignCenter,arr2[i]);
        x+=60;
    }

    //paint ships already placed on grid
    painter.translate(30,0);

    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(grid[i][j]!=empty)
            {
                painter.setBrush(Qt::darkGray);
                drawSquare(painter,i*60,j*60);
            }
        }
    }

    //paint current ship
    if (curShip->getType() != empty)
    {
        for (int i = 0; i < 5; ++i)
        {
            int x = curX + curShip->x(i); //different X coordinate for each square of curShip
            int y = curY - curShip->y(i); //different Y coordinate for each square of curShip
            painter.setBrush(Qt::black);
            drawSquare(painter, x *60, y*60);
        }
    }
}

void SetupScreen::keyPressEvent(QKeyEvent *event)
{
    if(currentFocusShips) return; // if no ship current selected return
    switch (event->key())
    {
        case Qt::Key_Left:
            tryMove(curShip, curX-1,curY);
            break;
        case Qt::Key_Right:
            tryMove(curShip, curX+1,curY);
            break;
        case Qt::Key_Down:
            tryMove(curShip, curX,curY+1);
            break;
        case Qt::Key_Up:
            tryMove(curShip, curX,curY-1);
            break;
        case Qt::Key_R:
            curShip->rotate(); // rotate by changing curShip's squares' positions
            if(!tryMove(curShip,curX,curY))
            {
                curShip->rotate();
                ui->Warning->setText("Not enough space to rotate!");
                displayWarning();
            }; // if out of bounds, rotate to original position
            break;
        case Qt::Key_Space: //shortcut for confirm button
            confirmPlacement();
            break;

        case Qt::Key_Escape:
            cancelPlacement();
            break;
    }
    repaint();
}

void SetupScreen::clearGrid()
{
    for(int i =0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            grid[i][j]=empty; //set all elements of grid to empty ShipType
        }
    }
}

void SetupScreen::drawSquare(QPainter &painter, int x, int y)
{
    QRectF test(x, y, 60, 60); // box height & width = 60
    painter.drawRect(test);
}

bool SetupScreen::tryMove(Ship *newPiece, int newX, int newY)
{
    for (int i = 0; i < 5; ++i)
    {
        int x = newX + newPiece->x(i);
        int y = newY - newPiece->y(i);

        if (x < 0 || x >= 10 || y < 0 || y >= 10) // if X or Y is not within 0-9
            return false; //new position ship is out of bounds
    }

    // new position ship is within grid bounds
    curX = newX; // set new curX
    curY = newY; // set new curY
    update();
    return true;
}

void SetupScreen::on_pushButton_2_clicked() //user clicks "cancel"
{
    cancelPlacement();
}

void SetupScreen::focusBoard()
{
    currentFocusShips = false;

    //disable ship buttons
    ui-> pushCarrier -> setEnabled(false);
    ui-> pushBattleship -> setEnabled(false);
    ui-> pushSubmarine -> setEnabled(false);
    ui-> pushDestroyer -> setEnabled(false);
    ui-> pushPatrol -> setEnabled(false);

    //enable "confirm", "cancel"
    ui-> pushButton -> setEnabled(true);
    ui-> pushButton_2 -> setEnabled(true);
}

void SetupScreen::focusShipButtons()
{
    currentFocusShips = true;

    //enable ship buttons
    ui-> pushCarrier -> setEnabled(true);
    ui-> pushBattleship -> setEnabled(true);
    ui-> pushSubmarine -> setEnabled(true);
    ui-> pushDestroyer -> setEnabled(true);
    ui-> pushPatrol -> setEnabled(true);

    //disable "confirm", "cancel"
    ui-> pushButton -> setEnabled(false);
    ui-> pushButton_2 -> setEnabled(false);
}

bool SetupScreen::checkSetupDone()
{
    bool b = true;
    for (int i=0;i<6;i++) //checks all elements in array
    {
        if(checkShips[i]==false) //if one ship is false, setup not done
            b=false;
    }
    return b;
}

void SetupScreen::confirmPlacement()
{
    for(int i =0; i<5; i++)
    {
        if (grid[curX + curShip->x(i)][curY - curShip->y(i)]!=empty)
        {
            ui->Warning->setText("Ships cannot overlap!");
            displayWarning();
            return;
        }
    }
    //for all 5 squares of curShip, store their X,Y coordinates (0-9) in the grid[10][10]
    for(int i =0; i<5; i++)
    {
        grid[curX + curShip->x(i)][curY - curShip->y(i)]=curShip->getType();
    }

    //change current ship type to placed
    checkShips[curShip->getType()] = true; // gets index position from ShipType enum in ship.h

    //reset curShip to empty
    delete curShip;
    curShip = new Ship(empty);
    repaint();

    //makes sure user can't press confirm, cancel, while no ship selected
    focusShipButtons();

    //checks if all ships are placed after every confirm
    if (checkSetupDone())
    {
       data = new matrix(grid);
       emit moveNext(player,*data);
    }
}

void SetupScreen::cancelPlacement()
{
    delete curShip;
    curShip = new Ship(empty);
    repaint();
    curButton->show(); // reshow current ship button
    focusShipButtons();
}

void SetupScreen::displayWarning()
{
    ui->Warning->show();
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->Warning->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(2000);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
}

SetupScreen::~SetupScreen()
{
    delete ui;
    delete data;
    delete curButton;
    delete curShip;
}
