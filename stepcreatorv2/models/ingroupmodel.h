#ifndef INGROUPMODEL_H
#define INGROUPMODEL_H

#include "models/abstractmodel.h"

class INGroupModel : public AbstractModel
{
public:
    INGroupModel();

    virtual AbstractModel::ModelType getModelType() {return AbstractModel::M_Group_IN;}
    virtual QString getName();

    virtual QString getInModelsDefinition();
    virtual QString getInComputeContent();
};

#endif // INGROUPMODEL_H
