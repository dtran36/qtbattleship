#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize member data
    firstscreen = new MainMenu;
    singleplayerSetup = new SetupScreen;
    versusSetup1 = new SetupScreen;
    versusSetup2 = new SetupScreen(2);
    versusGame = new GameScreen;
    singleplayerGame = new singleplayer;

    //add screens to stacked widget
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(firstscreen);
    stackedWidget->addWidget(singleplayerSetup);
    stackedWidget->addWidget(versusSetup1);
    stackedWidget->addWidget(versusSetup2);
    stackedWidget->addWidget(versusGame);
    stackedWidget->addWidget(singleplayerGame);

    //connect first screen buttons to slots
    connect(firstscreen,SIGNAL(clickedSingleplayer()),this,SLOT(switchSingleplayerSetup()));
    connect(firstscreen,SIGNAL(clickedVersus()),this,SLOT(switchSetup1()));
    connect(firstscreen,SIGNAL(clickedQuit()),qApp,SLOT(quit()));

    //connect move next signals to slots
    connect(singleplayerSetup,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchSingleplayerGame()));
    connect(versusSetup1,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchSetup2()));
    connect(versusSetup2,SIGNAL(moveNext(int,const matrix&)),this,SLOT(switchVersusGame()));

    //when setup is done, transfers grid data to GameScreen
    connect(singleplayerSetup,SIGNAL(moveNext(int,const matrix&)),singleplayerGame,SLOT(setGrid(int,const matrix&)));
    connect(versusSetup1,SIGNAL(moveNext(int,const matrix&)),versusGame,SLOT(setGrid(int,const matrix&)));
    connect(versusSetup2,SIGNAL(moveNext(int,const matrix&)),versusGame,SLOT(setGrid(int,const matrix&)));

    setCentralWidget(stackedWidget);
}

void MainWindow::switchMainMenu(){stackedWidget->setCurrentIndex(0);}
void MainWindow::switchSingleplayerSetup(){stackedWidget->setCurrentIndex(1);}
void MainWindow::switchSetup1(){stackedWidget->setCurrentIndex(2);}
void MainWindow::switchSetup2(){stackedWidget->setCurrentIndex(3);}
void MainWindow::switchVersusGame(){stackedWidget->setCurrentIndex(4);}
void MainWindow::switchSingleplayerGame(){stackedWidget->setCurrentIndex(5);}

MainWindow::~MainWindow()
{
    //deleting pointers causes crash
    delete ui;
}
