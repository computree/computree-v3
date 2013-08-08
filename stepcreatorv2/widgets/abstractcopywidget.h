#ifndef ABSTRACTCOPYWIDGET_H
#define ABSTRACTCOPYWIDGET_H

#include <QWidget>

class AbstractCopyModel;

class AbstractCopyWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractCopyWidget(AbstractCopyModel* model, QWidget *parent = 0);


    virtual QString getAlias() = 0;
    virtual QString getDisplayableName() = 0;
    virtual bool isvalid() = 0;

protected:
    AbstractCopyModel*      _model;

};

#endif // ABSTRACTCOPYWIDGET_H
