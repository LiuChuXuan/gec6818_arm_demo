#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QtDebug>

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setGeometry(0, 0, 800, 480);

    keyboard = new Keyboard(this);
    keyboard->changeGeometry(0, 240, 650, 240);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,
            this, &TcpClient::readData);

    lastSenderIsMe = true;
    isLinked = false;
}

TcpClient::~TcpClient()
{
    delete ui;
    delete socket;
    delete keyboard;
}

void TcpClient::readData()
{
    if(!isLinked)
    {
        isLinked = true;
        ui->textBrowser->append("物业:");
    }
    else if(lastSenderIsMe)
    {
        ui->textBrowser->append("\n物业:");
    }
    QString msg = socket->readAll();
    ui->textBrowser->append(msg);
    lastSenderIsMe = false;
}


void TcpClient::on_connectServer_clicked()
{
    socket->connectToHost(ui->ipEdit->text(),
                          ui->portEdit->text().toUShort());
}

void TcpClient::on_sendServer_clicked()
{
    if(!isLinked)
    {
        QMessageBox error(this);
        error.critical(this,"error","尚未连接到服务器");
        return;
    }
    socket->write(ui->textEdit->toPlainText().toUtf8());

    if(0 != strlen(ui->textEdit->toPlainText().toLatin1().data()))
    {
        if(!lastSenderIsMe)
        {
            ui->textBrowser->append("\n我:");
        }
        ui->textBrowser->append(ui->textEdit->toPlainText().toUtf8());
        lastSenderIsMe = true;
    }
    ui->textEdit->clear();
}

void TcpClient::on_clearBrowser_clicked()
{
    ui->textBrowser->clear();
}

void TcpClient::on_progress_clicked()
{

}

void TcpClient::on_back_clicked()
{
    this->hide();
    emit back();
}
