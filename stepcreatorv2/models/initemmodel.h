#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abstractmodel.h"

class INItemModel : public AbstractModel
{
public:
    INItemModel();

    virtual AbstractModel::ModelType getModelType() {return AbstractModel::M_Item_IN;}
    virtual QString getName();

    virtual QString getInModelsDefinition();
    virtual QString getInComputeContent();
};

#endif // INITEMMODEL_H
