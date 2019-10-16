#ifndef PUNCTUATIONSOFTKEY_H
#define PUNCTUATIONSOFTKEY_H

#include <QWidget>

namespace Ui
{
class PunctuationSoftkey;
}

class PunctuationSoftkey : public QWidget
{
    Q_OBJECT

public:
    explicit PunctuationSoftkey(QWidget *parent = nullptr);
    ~PunctuationSoftkey();

private slots:
    void showThis();
    void on_back_clicked();
    void changeGeometrySlot(int ax, int ay, int aw, int ah);


signals:
    void changeToLetter();

private:
    Ui::PunctuationSoftkey *ui;
};

#endif // PUNCTUATIONSOFTKEY_H
