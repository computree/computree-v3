#ifndef INRESULTWIDGET_H
#define INRESULTWIDGET_H

#include <QWidget>

namespace Ui {
    class INResultWidget;
}

class INResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit INResultWidget(QWidget *parent = 0);
    ~INResultWidget();

private slots:
    void on_rb_notneededresult_toggled(bool checked);

private:
    Ui::INResultWidget *ui;
};

#endif // INRESULTWIDGET_H
