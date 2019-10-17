#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>
#include "tcpclient.h"
#include "weatherdatetime.h"
#include <QTimerEvent>

namespace Ui
{
class MainInterface;
}

class MainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
    void hideThis();
    void timerEvent(QTimerEvent *e);

private:
    Ui::MainInterface *ui;
    TcpClient *tcpClient;
    WeatherDateTime *weather;
    int tid;
    int count;

signals:
    void logout();

private slots:
    void setLabelUserName(QString name);
    void showThis();
    void on_logOut_clicked();
    void on_repair_clicked();
    void on_weatherButton_clicked();
    void on_LEDControl_clicked();
    void on_camera_clicked();
};

#endif // MAININTERFACE_H
