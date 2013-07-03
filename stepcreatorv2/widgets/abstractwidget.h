#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>


class AbstractWidget : public QWidget
{
public:
    AbstractWidget(QWidget *parent);

    virtual QString getAlias() = 0;
    virtual bool isvalid() = 0;

};

#endif // ABSTRACTWIDGET_H
