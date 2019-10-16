#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include "softkey.h"
#include "punctuationsoftkey.h"

namespace Ui
{
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = nullptr);
    ~Keyboard();
    void changeGeometry(int ax, int ay, int aw, int ah);

private:
    Ui::Keyboard *ui;
    Softkey *softkey;
    PunctuationSoftkey *punc;

signals:
    void changeGeometrySignal(int ax, int ay, int aw, int ah);

};

#endif // KEYBOARD_H
