#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>

class AbstractModel;

class AbstractWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractWidget(AbstractModel* model, QWidget *parent = 0);


    virtual QString getAlias() = 0;
    virtual bool isvalid() = 0;

protected:
    AbstractModel*      _model;

};

#endif // ABSTRACTWIDGET_H
