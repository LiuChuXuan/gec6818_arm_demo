#ifndef BUTTON_H
#define BUTTON_H

#include <QToolButton>
class Button : public QToolButton
{
    Q_OBJECT
public:
    Button();
    explicit Button(QWidget *parent = nullptr);
    ~Button();
private slots:
    void button_clicked();
};

#endif // BUTTON_H
