#ifndef SOFTKEY_H
#define SOFTKEY_H

#include <QWidget>
#include "punctuationsoftkey.h"
#include <QDebug>

namespace Ui
{
class Softkey;
}

class Softkey : public QWidget
{
    Q_OBJECT

public:
    explicit Softkey(QWidget *parent = nullptr);
    ~Softkey();

private slots:
    void on_Punctuation_clicked();
    void showThis();
    void changeGeometrySlot(int ax, int ay, int aw, int ah);

signals:
    void changeToPunc();

private:
    Ui::Softkey *ui;
};

#endif // SOFTKEY_H
