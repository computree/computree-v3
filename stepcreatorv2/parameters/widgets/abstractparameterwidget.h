#ifndef ABSTRACTPARAMETERWIDGET_H
#define ABSTRACTPARAMETERWIDGET_H

#include <QWidget>

class AbstractParameter;

class AbstractParameterWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractParameterWidget(AbstractParameter* model, QWidget *parent = 0);

    virtual bool isvalid() = 0;
    virtual QString getName() = 0;

protected:
    AbstractParameter*      _model;

};

#endif // ABSTRACTPARAMETERWIDGET_H
