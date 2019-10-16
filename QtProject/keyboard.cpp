#include "keyboard.h"
#include "ui_keyboard.h"

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard)
{
    ui->setupUi(this);

    //符号
    punc = new PunctuationSoftkey(this);
    punc->hide();

    //字母
    softkey = new Softkey(this);

    //切换字母和符号键盘
    connect(softkey, SIGNAL(changeToPunc()),
            punc, SLOT(showThis()));
    connect(punc, SIGNAL(changeToLetter()),
            softkey, SLOT(showThis()));

    //改变键盘大小
    connect(this, SIGNAL(changeGeometrySignal(int, int, int, int)),
            punc, SLOT(changeGeometrySlot(int, int, int, int)));
    connect(this, SIGNAL(changeGeometrySignal(int, int, int, int)),
            softkey, SLOT(changeGeometrySlot(int, int, int, int)));

    softkey->show();

}

Keyboard::~Keyboard()
{
    delete ui;
    delete softkey;
    delete punc;

}

void Keyboard::changeGeometry(int ax, int ay, int aw, int ah)
{
    setGeometry(ax, ay, aw, ah);
    emit changeGeometrySignal(0, 0, aw, ah);
}
