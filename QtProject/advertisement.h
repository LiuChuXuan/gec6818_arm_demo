#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <QWidget>
#include <QTimer>
#include <QString>

namespace Ui {
class Advertisement;
}

class Advertisement : public QWidget
{
    Q_OBJECT

public:
    explicit Advertisement(QWidget *parent = nullptr);
    ~Advertisement();

private slots:
    void hide();
    void setText();
    void on_closeAdv_clicked();

signals:
    void closeSignal();

private:
    Ui::Advertisement *ui;
    QTimer *timer;
    QTimer *timer2;
    int count;
};

#endif // ADVERTISEMENT_H
