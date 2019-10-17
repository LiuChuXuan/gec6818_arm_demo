#include "maininterface.h"
#include "ui_maininterface.h"
MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);

    tcpClient = new TcpClient(this);
    tcpClient->hide();

    connect(tcpClient, SIGNAL(back()),
            this, SLOT(showThis()));

    weather = new WeatherDateTime(this);
    weather->hide();
    connect(weather, SIGNAL(back()),
            this, SLOT(showThis()));

    ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/1.jpg);");
    tid  = startTimer(3000);//每三秒自动切换广告timerEvent()
    count = 1;
}

MainInterface::~MainInterface()
{
    delete ui;
    delete tcpClient;
    delete weather;
}

void MainInterface::hideThis()
{
    ui->camera->hide();
    ui->LEDControl->hide();
    ui->autoAdvertisement->hide();
    ui->userName->hide();
    ui->repair->hide();
    ui->weatherButton->hide();
    ui->logOut->hide();
}

void MainInterface::timerEvent(QTimerEvent *e)
{
    switch(++count)
    {
    case 1:
        ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/1.jpg);");
        break;

    case 2:
        ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/2.jpg);");
        break;

    case 3:
        ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/3.jpg);");
        break;

    case 4:
        ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/4.jpg);");
        break;

    case 5:
        ui->autoAdvertisement->setStyleSheet("border-image: url(:/image/5.jpg);");
        count = 0;
        break;
    }
}

void MainInterface::setLabelUserName(QString name)
{
    ui->userName->setText(" 用户名：\n " + name);
    ui->userName->show();
}


void MainInterface::showThis()
{
    ui->camera->show();
    ui->LEDControl->show();
    ui->autoAdvertisement->show();
    ui->repair->show();
    ui->weatherButton->show();
    ui->userName->show();
    ui->logOut->show();
}

void MainInterface::on_logOut_clicked()
{
    //在mainwindow中connect
    emit logout();
}

void MainInterface::on_repair_clicked()
{
    hideThis();
    tcpClient->show();
}

void MainInterface::on_weatherButton_clicked()
{
    hideThis();
    weather->show();
}

void MainInterface::on_LEDControl_clicked()
{

}

void MainInterface::on_camera_clicked()
{

}
