#ifndef WEATHERDATETIME_H
#define WEATHERDATETIME_H

#include <QWidget>

namespace Ui {
class WeatherDateTime;
}

class WeatherDateTime : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherDateTime(QWidget *parent = nullptr);
    ~WeatherDateTime();

private slots:
    void on_back_clicked();

signals:
    void back();

private:
    Ui::WeatherDateTime *ui;
};

#endif // WEATHERDATETIME_H
