#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QWidget>
#include <QPushButton>
#include "loginedit.h"
#include <QFile>
#include <cstring>
#include <QMessageBox>

struct accountInfo
{
    char account[16];
    char password[16];
};

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QWidget
{
    Q_OBJECT

public:
    explicit LoginInterface(QWidget *parent = nullptr);
    ~LoginInterface();

private slots:
    void on_logIn_clicked();
    void on_signUp_clicked();

signals:
    void logInSuccess(QString userName);

private:
    Ui::LoginInterface *ui;
    LoginEdit *loginEdit;
};

#endif // LOGININTERFACE_H
