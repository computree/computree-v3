#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>

class AbstractInModel;

class AbstractInWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractInWidget(AbstractInModel* model, QWidget *parent = 0);


    virtual QString getAlias() = 0;
    virtual bool isvalid() = 0;

protected:
    AbstractInModel*      _model;

};

#endif // ABSTRACTWIDGET_H
