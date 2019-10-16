#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>
#include <QLabel>
#include <QDateTime>
#include "tcpclient.h"

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
    void setDateTime();

private:
    QLabel *userName;
   // QLabel *date_time;
   // QDateTime *dateTime;
    Ui::MainInterface *ui;
    TcpClient *tcpClient;

private slots:
    void setLabelUserName(QString name);
    void on_pushButton_clicked();
    void showThis();
};

#endif // MAININTERFACE_H
