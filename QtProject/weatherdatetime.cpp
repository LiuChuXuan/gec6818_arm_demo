#include "weatherdatetime.h"
#include "ui_weatherdatetime.h"

#include <QtDebug>

WeatherDateTime::WeatherDateTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherDateTime)
{
    ui->setupUi(this);


    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished,
            this, &WeatherDateTime::readData);
    QUrl url("http://t.weather.sojson.com/api/weather/city/101010100");
    QNetworkRequest request(url);
    manager->get(request);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(showCurrentTime()));

}

WeatherDateTime::~WeatherDateTime()
{
    delete ui;
    delete manager;
}

QString WeatherDateTime::getCurrentTime()
{
    QUrl timrUrl("http://quan.suning.com/getSysTime.do");
    QNetworkAccessManager timeManager;
    QEventLoop loop;

    QNetworkReply *timeReply = timeManager.get(QNetworkRequest(timrUrl));  //发送get请求
    connect(timeReply, &QNetworkReply::finished, &loop, &QEventLoop::quit); //请求结束并下载完成后，退出子事件循环

    //开启子事件循环
    loop.exec();

    QString timeString = timeReply->readAll();

    QDate currentDate(timeString.mid(13, 4).toInt(),
                      timeString.mid(18, 2).toInt(),
                      timeString.mid(21, 2).toInt());

    QTime currentTime(timeString.mid(24, 2).toInt(),
                      timeString.mid(27, 2).toInt(),
                      timeString.mid(30, 2).toInt());

    currentDateTime.setDate(currentDate);
    currentDateTime.setTime(currentTime);

    timeReply->deleteLater();
    return timeString;
}


void WeatherDateTime::on_back_clicked()
{
    //在父类maininterface中connect
    hide();
    emit back();
}

void WeatherDateTime::readData(QNetworkReply *reply)
{
    // 读取服务器数据
    QByteArray array = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(array);

    // 解析json文档
    QJsonObject object = doc.object();

    // 城市名
    QJsonObject cityObject = object.value("cityInfo").toObject();
    currentCity = object.value("cityInfo").toObject().value("parent").toString() + "省 " +
                  cityObject.value("city").toString();

    // 当前星期
    QJsonObject data = object.value("data").toObject();
    QJsonArray forecastArray = data.value("forecast").toArray();
    QJsonObject one = forecastArray.at(0).toObject();
    currentWeek = one.value("week").toString();

    getCurrentTime();
    timer->start(1000);

    ui->current_2->setText(" 湿度:" + data.value("shidu").toString() +
                           "  pm2.5: " + QString::number(data.value("pm25").toInt()) +
                           "  空气质量:" + data.value("quality").toString() +
                           "  温度:" + data.value("wendu").toString());

    //ui->tableWidget->setItem()
    int count = 0;

    for(count = 0; count < forecastArray.size(); count++)
    {
        QJsonObject dateobj = forecastArray.at(count).toObject();

        QString weather = dateobj.value("type").toString();//天气
        QString aqi = QString::number(dateobj.value("sqi").toInt()); //空气质量指数
        QString high = dateobj.value("high").toString();//最高温
        QString low = dateobj.value("low").toString();//最低温
        QString sunrise = dateobj.value("sunrise").toString();//日升时间
        QString sunset = dateobj.value("sunset").toString();//日落时间
        QString fx = dateobj.value("fx").toString();//风向
        QString fl = dateobj.value("fl").toString();//风强

        ui->tableWidget->setItem(0, count, new QTableWidgetItem(weather)); //天气
        ui->tableWidget->setItem(1, count, new QTableWidgetItem(aqi)); //空气质量指数
        ui->tableWidget->setItem(2, count, new QTableWidgetItem(high)); //最高温
        ui->tableWidget->setItem(3, count, new QTableWidgetItem(low)); //最低温
        ui->tableWidget->setItem(4, count, new QTableWidgetItem(sunrise)); //日升时间
        ui->tableWidget->setItem(5, count, new QTableWidgetItem(sunset)); //日落时间
        ui->tableWidget->setItem(6, count, new QTableWidgetItem(fx)); //风向
        ui->tableWidget->setItem(7, count, new QTableWidgetItem(fl)); //风强
    }

    reply->deleteLater();
}

void WeatherDateTime::showCurrentTime()
{
    QString year = QString::number(currentDateTime.date().year());
    QString month = QString::number(currentDateTime.date().month());
    QString day = QString::number(currentDateTime.date().day());
    QString hour = QString::number(currentDateTime.time().hour());
    QString minute = QString::number(currentDateTime.time().minute());
    QString second = QString::number(currentDateTime.time().second());

    ui->current->setText(" " + currentCity + " " +
                         year + "-" + month + "-" + day + " " +
                         hour + ":" + minute + ":" + second + " " +
                         currentWeek);

    currentDateTime = currentDateTime.addSecs(1);
}



