#include "maininterface.h"
#include "ui_maininterface.h"
MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    userName = new QLabel(this);
    userName->setGeometry(0, 0, 800, 30);

    tcpClient = new TcpClient(this);
    tcpClient->hide();

    connect(tcpClient, SIGNAL(back()),
            this, SLOT(showThis()));

    //setDateTime();
}

MainInterface::~MainInterface()
{
    delete ui;
    delete userName;
    delete tcpClient;

}

void setDateTime()
{
    //dateTime = new QDateTime(QDateTime::currentDateTime());
    //QString qStr = dateTime.toString("yyy-MM-dd hh:mm::ss ddd");
}

void MainInterface::setLabelUserName(QString name)
{
    userName->setText("用户名：" + name);
    userName->show();
}

void MainInterface::on_pushButton_clicked()
{
    ui->pushButton->hide();
    userName->hide();
    ui->logOut->hide();
    tcpClient->show();
}

void MainInterface::showThis()
{
    ui->pushButton->show();
    userName->show();
    ui->logOut->show();
}
