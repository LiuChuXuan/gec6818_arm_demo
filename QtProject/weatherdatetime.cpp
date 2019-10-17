#include "weatherdatetime.h"
#include "ui_weatherdatetime.h"

WeatherDateTime::WeatherDateTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherDateTime)
{
    ui->setupUi(this);
}

WeatherDateTime::~WeatherDateTime()
{
    delete ui;
}


void WeatherDateTime::on_back_clicked()
{
    //在父类maininterface中connect
    hide();
    emit back();
}
