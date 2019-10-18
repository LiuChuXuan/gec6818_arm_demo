#ifndef WEATHERDATETIME_H
#define WEATHERDATETIME_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTimer>

namespace Ui
{
class WeatherDateTime;
}

class WeatherDateTime : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherDateTime(QWidget *parent = nullptr);
    ~WeatherDateTime();
    QString getCurrentTime();

private slots:
    void on_back_clicked();
    void readData(QNetworkReply* reply);
    void showCurrentTime();

signals:
    void back();

private:
    Ui::WeatherDateTime *ui;
    QNetworkAccessManager* manager;
    QDateTime currentDateTime;
    QString currentCity;
    QString currentWeek;
    QTimer *timer;
};

#endif // WEATHERDATETIME_H
