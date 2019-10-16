#include "button.h"

#include <QApplication>
#include <QKeyEvent>
Button::Button(QWidget *parent): QToolButton(parent)
{
    //把按钮自己的点击信号关联自己的槽函数button_clicked
    connect(this, &Button::clicked, this, &Button::button_clicked);
}
Button::~Button()
{
}

void Button::button_clicked()
{
    //创建按键事件
    QString ch = this->text();
    QKeyEvent *key;

    if(ch == "<-")
    {
        key = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, ch);
    }
    else
    {
        char c = ch.at(0).toLatin1();
        key = new QKeyEvent(QKeyEvent::KeyPress, c, Qt::NoModifier, ch);
    }

    //通过QApplication把事件添加到队列--派发
    QApplication::postEvent(QApplication::focusWidget(), key);
}
