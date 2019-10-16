#include "logininterface.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginInterface)
{
    ui->setupUi(this);
    setGeometry(0, 0, 800, 480);

    loginEdit = new LoginEdit(this);
    loginEdit->show();

    ui->signUp->setGeometry(400 - 100 - 30, 200, 100, 60);
    ui->logIn->setGeometry(400        + 30, 200, 100, 60);

}

LoginInterface::~LoginInterface()
{
    delete ui;
    delete loginEdit;
}

void LoginInterface::on_logIn_clicked()
{
    //将输入的信息转换成char *型
    char *account = loginEdit->getAccount().toLatin1().data();
    char *password = loginEdit->getPassword().toLatin1().data();
    //定义一个16个字节的字符数组并且将它清零
    char accountArr[16];
    char passwordArr[16];
    memset(accountArr, 0, 16);
    memset(passwordArr, 0, 16);
    //将有效位复制到数组中,这样就能保证其余位都为'\0'
    strncpy(accountArr, account, strlen(account));
    strncpy(passwordArr, password, strlen(account));

    QFile accountRead("./accountinfo.txt");

    char accountBuf[16];
    char passwordBuf[16];

    if(accountRead.exists())
    {
        accountRead.open(QIODevice::ReadOnly);

        while(accountRead.read(accountBuf, 16))
        {
            accountRead.read(passwordBuf, 16);

            if(!strncmp(accountBuf, accountArr, 16))
            {
                //两个账号相同则进入这个判断中
                if(!strncmp(passwordBuf, passwordArr, 16))
                {
                    //两个密码相同则进入这个判断(登录成功)
                    emit logInSuccess(account);
                    return;
                }
                else
                {
                    QMessageBox tips(this);
                    tips.warning(this, "登录失败", "密码错误");
                    return;
                }
            }
        }
    }

    QMessageBox tips(this);
    tips.warning(this, "登录失败", "没有该账号");

}

void LoginInterface::on_signUp_clicked()
{
    bool isAccount = true;
    bool accountExit = false;
    QFile accountRead("./accountinfo.txt");

    //将输入的信息转换成char *型
    char *account = loginEdit->getAccount().toLatin1().data();
    char *password = loginEdit->getPassword().toLatin1().data();
    //定义一个16个字节的字符数组并且将它清零
    char accountArr[16];
    char passwordArr[16];
    memset(accountArr, 0, 16);
    memset(passwordArr, 0, 16);
    //将有效位复制到数组中,这样就能保证其余位都为'\0'
    strncpy(accountArr, account, strlen(account));
    strncpy(passwordArr, password, strlen(account));

    char buf[16];

    if(accountRead.exists())
    {
        accountRead.open(QIODevice::ReadOnly);

        while(accountRead.read(buf, 16))
        {
            if(isAccount)
            {
                //如果是账号，则判断这个账号有没有在文件中注册过
                if(!strncmp(buf, accountArr, 16))
                {
                    //两个账号相同则进入这个判断中
                    accountExit = true;
                    QMessageBox tips(this);
                    tips.warning(this, "注册失败", "该账号已存在");
                    accountRead.close();
                    return;
                }
            }

            isAccount = !isAccount;
        }

        accountRead.close();

        if(!accountExit)
        {
            QFile accountAppend("./accountinfo.txt");
            accountAppend.open(QIODevice::Append);
            accountAppend.write(accountArr, 16);
            accountAppend.write(passwordArr, 16);
            accountAppend.close();
            QMessageBox tips(this);
            tips.warning(this, "注册成功", "注册成功");
        }
    }
    else
    {
        QFile accountWrite("./accountinfo.txt");
        accountWrite.open(QIODevice::WriteOnly);
        accountWrite.write(accountArr, 16);
        accountWrite.write(passwordArr, 16);
        accountWrite.close();
        QMessageBox tips(this);
        tips.warning(this, "注册成功", "\n    注册成功    \n");
    }
}
