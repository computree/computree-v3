#ifndef GSTEPSDOCKWIDGET_H
#define GSTEPSDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class GStepsDockWidget;
}

class GStepsDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit GStepsDockWidget(QWidget *parent = 0);
    ~GStepsDockWidget();

    void init();

private:
    Ui::GStepsDockWidget *ui;
};

#endif // GSTEPSDOCKWIDGET_H
