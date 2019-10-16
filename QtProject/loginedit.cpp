#include "loginedit.h"
#include "ui_loginedit.h"

LoginEdit::LoginEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginEdit)
{
    ui->setupUi(this);
    setGeometry(150, 30, 500, 150);

    ui->pwEdit->setMaxLength(16);
    ui->userEdit->setMaxLength(16);

}

LoginEdit::~LoginEdit()
{
    delete ui;
}

QString LoginEdit::getPassword()
{
    return ui->pwEdit->text();
}

QString LoginEdit::getAccount()
{
    return ui->userEdit->text();
}
