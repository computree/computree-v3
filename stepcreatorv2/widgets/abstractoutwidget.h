#ifndef ABSTRACTOUTWIDGET_H
#define ABSTRACTOUTWIDGET_H

#include <QWidget>

class AbstractOutModel;

class AbstractOutWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractOutWidget(AbstractOutModel* model, QWidget *parent = 0);


    virtual QString getAlias() = 0;
    virtual QString getDisplayableName() = 0;
    virtual bool isvalid() = 0;

protected:
    AbstractOutModel*      _model;

};

#endif // ABSTRACTOUTWIDGET_H
