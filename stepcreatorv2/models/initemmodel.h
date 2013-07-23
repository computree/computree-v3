#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abstractinmodel.h"

class INItemModel : public AbstractInModel
{
public:
    INItemModel();

    virtual AbstractInModel::ModelType getModelType() {return AbstractInModel::M_Item_IN;}
    virtual QString getName();

    virtual void getInModelsIncludesList(QSet<QString> &list);
    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeContent();
};

#endif // INITEMMODEL_H
