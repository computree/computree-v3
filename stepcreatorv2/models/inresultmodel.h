#ifndef INRESULTMODEL_H
#define INRESULTMODEL_H

#include "models/abstractinmodel.h"

class INResultModel : public AbstractInModel
{
public:
    INResultModel();

    virtual AbstractInModel::ModelType getModelType() {return AbstractInModel::M_Result_IN;}
    virtual QString getName();
    virtual bool isValid();

    virtual void getInModelsIncludesList(QSet<QString> &list);
    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeContent();

};

#endif // INRESULTMODEL_H
