#ifndef INGROUPMODEL_H
#define INGROUPMODEL_H

#include "models/abstractinmodel.h"

class INGroupModel : public AbstractInModel
{
public:
    INGroupModel();

    virtual AbstractInModel::ModelType getModelType() {return AbstractInModel::M_Group_IN;}
    virtual QString getName();

    virtual void getInModelsIncludesList(QSet<QString> &list);
    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeContent();

};

#endif // INGROUPMODEL_H
