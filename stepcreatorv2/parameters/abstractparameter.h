#ifndef ABSTRACTPARAMETER_H
#define ABSTRACTPARAMETER_H

#include "qstandarditemmodel.h"
#include "parameters/widgets/abstractparameterwidget.h"

class AbstractParameter : public QStandardItem
{
public:

    AbstractParameter();
    ~AbstractParameter();

    virtual AbstractParameterWidget* getWidget();
    virtual bool isValid();
    virtual bool isDataParameter();
    virtual QString getName();


protected:
    AbstractParameterWidget*  _widget;

};
#endif // ABSTRACTPARAMETER_H
