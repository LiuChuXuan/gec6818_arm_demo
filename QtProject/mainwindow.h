#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "keyboard.h"//软键盘
#include "advertisement.h"//3s广告界面
#include "logininterface.h"//登录界面
#include "maininterface.h"

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
    void closeAdvertisement();
    void closeLoginInterface(QString userName);

signals:
    void setUserName(QString);

private:
    Ui::MainWindow *ui;
    Softkey *softkey;
    PunctuationSoftkey *punc;
    Advertisement *advertisement;
    LoginInterface *loginInterface;
    MainInterface *mainInterface;
    Keyboard *keyboard;
};

#endif // MAINWINDOW_H
