#include "punctuationsoftkey.h"
#include "ui_punctuationsoftkey.h"
#include <QDebug>

PunctuationSoftkey::PunctuationSoftkey(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PunctuationSoftkey)
{
    ui->setupUi(this);
}

PunctuationSoftkey::~PunctuationSoftkey()
{
    delete ui;
}

void PunctuationSoftkey::showThis()
{
    show();
}

void PunctuationSoftkey::on_back_clicked()
{
    hide();
    emit changeToLetter();
}

void PunctuationSoftkey::changeGeometrySlot(int ax, int ay, int aw, int ah)
{
    setGeometry(ax, ay, aw, ah);
}
