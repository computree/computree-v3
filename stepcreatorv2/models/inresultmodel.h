#ifndef INRESULTMODEL_H
#define INRESULTMODEL_H

#include "models/abstractmodel.h"

class INResultModel : public AbstractModel
{
public:
    INResultModel();

    virtual AbstractModel::ModelType getModelType() {return AbstractModel::M_Result_IN;}
    virtual QString getName();
    virtual bool isValid();

    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeContent();

};

#endif // INRESULTMODEL_H
