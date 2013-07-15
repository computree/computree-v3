#ifndef ABTRACTMODEL_H
#define ABTRACTMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractwidget.h"

class AbstractModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_IN,
        M_Group_IN,
        M_Item_IN,
        M_Result_OUT,
        M_Group_OUT,
        M_Item_OUT
    };

    AbstractModel();
    ~AbstractModel();

    virtual AbstractModel::ModelType getModelType() = 0;

    virtual AbstractWidget* getWidget();
    virtual QString getName() = 0;
    QString getDef();

    virtual QString getAlias();
    virtual bool isValid();

    virtual QString getInModelsDefinition() = 0;
    void getChildrenInModelsDefinitions(QString &result);

    virtual QString getInComputeContent() = 0;
    void getChildrenInComputeContent(QString &result);

    void onAliasChange();

protected:
    AbstractWidget*  _widget;

};

#endif // ABTRACTMODEL_H
