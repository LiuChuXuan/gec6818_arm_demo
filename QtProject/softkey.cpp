#include "softkey.h"
#include "ui_softkey.h"
#include "punctuationsoftkey.h"

Softkey::Softkey(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Softkey)
{
    ui->setupUi(this);
}

Softkey::~Softkey()
{
    delete ui;
}


void Softkey::on_Punctuation_clicked()
{
    hide();
    emit changeToPunc();
}

void Softkey::showThis()
{
    show();
}

void Softkey::changeGeometrySlot(int ax, int ay, int aw, int ah)
{
    setGeometry(ax, ay, aw, ah);
}




