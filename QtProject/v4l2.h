#ifndef V4L2_H
#define V4L2_H

#include <QWidget>

namespace Ui {
class V4L2;
}

class V4L2 : public QWidget
{
    Q_OBJECT

public:
    explicit V4L2(QWidget *parent = nullptr);
    ~V4L2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::V4L2 *ui;

};

#endif // V4L2_H
