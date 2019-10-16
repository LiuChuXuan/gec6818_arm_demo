#ifndef LOGINEDIT_H
#define LOGINEDIT_H

#include <QWidget>

namespace Ui {
class LoginEdit;
}

class LoginEdit : public QWidget
{
    Q_OBJECT

public:
    explicit LoginEdit(QWidget *parent = nullptr);
    ~LoginEdit();

    QString getPassword();
    QString getAccount();

private:
    Ui::LoginEdit *ui;
};

#endif // LOGINEDIT_H
