#ifndef ABSTRACTINWIDGET_H
#define ABSTRACTINWIDGET_H

#include <QWidget>

class AbstractInModel;

class AbstractInWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractInWidget(AbstractInModel* model, QWidget *parent = 0);


    virtual QString getAlias() = 0;
    virtual QString getDescription() = 0;
    virtual QString getDisplayableName() = 0;
    virtual bool isvalid() = 0;

protected:
    AbstractInModel*      _model;

};

#endif // ABSTRACTINWIDGET_H
