#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "keyboard.h"
#include <QMessageBox>
#include <cstring>


namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

private slots:
    void readData();
    void on_sendServer_clicked();
    void on_connectServer_clicked();
    void on_clearBrowser_clicked();
    void on_progress_clicked();
    void on_back_clicked();

signals:
    void back();

private:
    Ui::TcpClient *ui;
    QTcpSocket *socket;
    Keyboard *keyboard;
    bool lastSenderIsMe;
    bool isLinked;
};

#endif // TCPCLIENT_H
