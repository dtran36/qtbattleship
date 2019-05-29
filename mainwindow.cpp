#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize member data
    firstscreen = new MainMenu(this);
    singleplayerSetup = new SetupScreen(1,this);
    versusSetup1 = new SetupScreen(1,this);
    versusSetup2 = new SetupScreen(2,this);
    game = new GameScreen(this);
    bot = new NPC;
    dialogGameover = new Dialog;
    instructions = new Instructions;

    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/Music.mp3"));
    music->setVolume(2);
    music->play();

    //add screens to stacked widget
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(firstscreen);
    stackedWidget->addWidget(singleplayerSetup);
    stackedWidget->addWidget(versusSetup1);
    stackedWidget->addWidget(versusSetup2);
    stackedWidget->addWidget(game);

    //connect first screen
    connect(firstscreen,SIGNAL(clickedQuit()),qApp,SLOT(quit()));
    connect(firstscreen,SIGNAL(clickedSingleplayer()),this,SLOT(switchSingleplayerSetup()));
    connect(firstscreen,SIGNAL(clickedVersus()),this,SLOT(switchSetup1()));

    //connect dialog: quit
    connect(dialogGameover,SIGNAL(clk_Quit()),qApp,SLOT(quit()));

    connectAll();

    setCentralWidget(stackedWidget);

    QTimer::singleShot(1000, this, SLOT(showInstructions()));
}

void MainWindow::connectAll()
{
    //connect move next signals to slots
    connect(singleplayerSetup,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchGameScreen()));
    connect(versusSetup1,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchSetup2()));
    connect(versusSetup2,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchGameScreen()));

    //when setup is done, transfers grid data to GameScreen
    connect(singleplayerSetup,SIGNAL(moveNext(int,const matrix&)),game,SLOT(setGrid(int,const matrix&)));
    connect(versusSetup1,SIGNAL(moveNext(int,const matrix&)),game,SLOT(setGrid(int,const matrix&)));
    connect(versusSetup2,SIGNAL(moveNext(int,const matrix&)),game,SLOT(setGrid(int,const matrix&)));
    //set mode to versus
    connect(versusSetup2,SIGNAL(moveNext(int,const matrix&)),game,SLOT(setVersus()));

    //connect npc
    connect(singleplayerSetup,SIGNAL(moveNext(int,const matrix&)),bot,SLOT(getNPC()));
    connect(bot,SIGNAL(giveData(int,const matrix&)),game,SLOT(setGrid(int,const matrix&)));

    //connect dialog: go to Main Menu
    connect(game,SIGNAL(playerXWins(int)),dialogGameover,SLOT(slot_show(int)));
    connect(dialogGameover,SIGNAL(clk_Main()),this,SLOT(resetAll()));
    connect(dialogGameover,SIGNAL(clk_Main()),this,SLOT(switchMainMenu()));

    //connect: Toggle mute
    connect(versusSetup1,SIGNAL(mutePressed()),this,SLOT(toggleMusic()));
    connect(versusSetup2,SIGNAL(mutePressed()),this,SLOT(toggleMusic()));
    connect(singleplayerSetup,SIGNAL(mutePressed()),this,SLOT(toggleMusic()));
    connect(game,SIGNAL(mutePressed()),this,SLOT(toggleMusic()));
}

void MainWindow::resetAll()
{
    stackedWidget->removeWidget(singleplayerSetup);
    stackedWidget->removeWidget(versusSetup1);
    stackedWidget->removeWidget(versusSetup2);
    stackedWidget->removeWidget(game);

    delete singleplayerSetup;
    delete versusSetup1;
    delete versusSetup2;
    delete game;
    delete bot;

    singleplayerSetup = new SetupScreen(1,this);
    versusSetup1 = new SetupScreen(1,this);
    versusSetup2 = new SetupScreen(2,this);
    game = new GameScreen(this);
    bot = new NPC;

    stackedWidget->addWidget(singleplayerSetup);
    stackedWidget->addWidget(versusSetup1);
    stackedWidget->addWidget(versusSetup2);
    stackedWidget->addWidget(game);

    connectAll();
}

void MainWindow::switchMainMenu(){stackedWidget->setCurrentIndex(0);}
void MainWindow::switchSingleplayerSetup(){stackedWidget->setCurrentIndex(1);}
void MainWindow::switchSetup1(){stackedWidget->setCurrentIndex(2);}
void MainWindow::switchSetup2(){stackedWidget->setCurrentIndex(3);}
void MainWindow::switchGameScreen(){stackedWidget->setCurrentIndex(4);}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_M)
    {
        toggleMusic();
    }
}

void MainWindow::toggleMusic()
{
    if(!mute)
    {
        music->pause();
        mute = true;
    }
    else {
        music->play();
        mute = false;
    }
}

void MainWindow::showInstructions()
{
    instructions->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
