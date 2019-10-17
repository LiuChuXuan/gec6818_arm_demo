#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0, 0, 800, 480);

    //创建登录输入框
    loginInterface = new LoginInterface(this);
    connect(loginInterface, SIGNAL(logInSuccess(QString)),
            this, SLOT(closeLoginInterface(QString)));
    loginInterface->show();

    //创建软键盘
    keyboard = new Keyboard(this);
    keyboard->changeGeometry(50, 300, 700, 180);
    keyboard->show();

    //创建广告页面
    advertisement = new Advertisement(this);
    connect(advertisement, SIGNAL(closeSignal()),
            this, SLOT(closeAdvertisement()));

    //主界面
    mainInterface = new MainInterface(this);
    mainInterface->hide();
    connect(this, SIGNAL(setUserName(QString)),
            mainInterface, SLOT(setLabelUserName(QString)));
    connect(mainInterface, SIGNAL(logout()),
            this, SLOT(userLogout()));

    advertisement->show();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete advertisement;
    delete loginInterface;
    delete mainInterface;
}

void MainWindow::closeAdvertisement()
{
    advertisement->close();
}


void MainWindow::closeLoginInterface(QString userName)
{
    //关闭软键盘和输入界面
    //loginInterface->hide();
    //keyboard->hide();
    delete loginInterface;
    loginInterface = nullptr;
    delete keyboard;
    keyboard = nullptr;

    emit setUserName(userName);
    mainInterface->show();
}

void MainWindow::userLogout()
{
    delete mainInterface;

    //创建登录输入框
    loginInterface = new LoginInterface(this);
    connect(loginInterface, SIGNAL(logInSuccess(QString)),
            this, SLOT(closeLoginInterface(QString)));
    loginInterface->show();

    //创建软键盘
    keyboard = new Keyboard(this);
    keyboard->changeGeometry(50, 300, 700, 180);
    keyboard->show();

    //主界面
    mainInterface = new MainInterface(this);
    mainInterface->hide();
    connect(this, SIGNAL(setUserName(QString)),
            mainInterface, SLOT(setLabelUserName(QString)));
    connect(mainInterface, SIGNAL(logout()),
            this, SLOT(userLogout()));

    loginInterface->show();
    keyboard->show();
}

