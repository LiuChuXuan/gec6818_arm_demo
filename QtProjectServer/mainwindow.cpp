#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&server, &QTcpServer::newConnection, this, &MainWindow::newClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_userList_clicked()
{

}

void MainWindow::on_connectServer_clicked()
{
    if(ui->portEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "输入提示", "请输入要监听的端口号");
        return;
    }

    //[2]监听客户端
    //bool QTcpServer::listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0)
    server.listen(QHostAddress::Any, ui->portEdit->text().toUShort()); //启动服务器
    QMessageBox tips;
    tips.information(this, "启动成功", "启动成功");

}

void MainWindow::on_clearBrowser_clicked()
{
    ui->chatBrowser->clear();
}

void MainWindow::on_sendServer_clicked()
{
    //给选中客户端发送数据
    //获取当前列表选中的行号
    int row = ui->userList->currentRow();

    if(row < 0)
    {
        return;
    }

    //由row从sockets中查询到套接字
    QTcpSocket *socket = sockets.at(row);

    socket->write(ui->chatEdit->toPlainText().toUtf8());
    ui->chatEdit->clear();

}

void MainWindow::newClient()
{
    //[4]获取哪 一个客户端
    QTcpSocket *socket = server.nextPendingConnection(); //每次都会返回一个新的客户端,比如张三，李四

    //[5] 给这个客户端打一个招呼
    socket->write("连接成功");

    //当客户端有数据到达服务器对应的客户端套接字QTcpSocket对象会发送readyRead信号
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    //当socket对象掉线会发送disconnected信号
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::offline);

    //获取客户端ip地址
    QString ip = socket->peerAddress().toString();//获取客户端IP地址
    ip.remove("::ffff:");
    //把IP地址添加到列表中
    ui->userList->addItem(ip);

    //把套接字添加到列表中
    sockets.append(socket);

}

void MainWindow::readData()
{
    //在该函数中，要明白到底是那个一客户端发送的信息，在Qt中，提供一个发送者获取接口
    //QObject* sender()const;
    QTcpSocket* msocket = qobject_cast<QTcpSocket*>(sender()); //代表任何人
    QString msg = msocket->readAll(); //读取信息
    //获取对方的ip
    QString ip = msocket->peerAddress().toString();

    //在控件上显示用名和用户发送的信息
    ip = ip.remove(0, 7); //删除字符串指定长度的字符串
    ui->chatBrowser->append(ip + "\n" + msg + "\n");
}

void MainWindow::offline()
{
    //获取信号发送者
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QString ip = socket->peerAddress().toString();//获取客户端IP地址
    ip.remove("::ffff:");
    qDebug() << ip << "掉线";

    //根据掉线的套接字找到在列表中的行号,后再删除套接字
    int row = sockets.indexOf(socket);
    sockets.removeAt(row);

    //根据编号row删除userlist列表中的对应行
    QListWidgetItem *item = ui->userList->takeItem(row);
    delete item;
}
