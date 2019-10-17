#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_userList_clicked();
    void on_connectServer_clicked();
    void on_clearBrowser_clicked();
    void on_sendServer_clicked();

public slots:
    void newClient();
    void readData();
    void offline();

private:
    Ui::MainWindow *ui;
    QTcpServer server;

    //创建一个容器存储套接字
    QVector<QTcpSocket* > sockets;

};

#endif // MAINWINDOW_H
