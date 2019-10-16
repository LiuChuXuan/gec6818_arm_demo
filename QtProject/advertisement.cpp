#include "advertisement.h"
#include "ui_advertisement.h"

Advertisement::Advertisement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Advertisement)
{
    ui->setupUi(this);
    this->setGeometry(0,0,800,480);

    //定时三秒钟后关闭广告页面
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(hide()));
    timer->start(3000);

    //定时每一秒刷新显示倒计时
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(setText()));
    timer2->start(1000);
    count = 2;

}

Advertisement::~Advertisement()
{
    delete ui;
}

void Advertisement::hide()
{
    timer->stop();
    timer2->stop();
    emit closeSignal();
}

void Advertisement::setText()
{
    ui->closeAdv->setText("关闭广告 " + QString::number(count) + "s");
    count -= 1;

    if(count < 0)
    {
        timer2->stop();
    }
}

void Advertisement::on_closeAdv_clicked()
{
    timer->stop();
    timer2->stop();
    emit closeSignal();
}

