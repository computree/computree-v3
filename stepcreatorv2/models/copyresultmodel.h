#ifndef COPYRESULTMODEL_H
#define COPYRESULTMODEL_H

#include "models/abstractcopymodel.h"

class COPYResultModel : public AbstractCopyModel
{
public:
    COPYResultModel();

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Result_COPY;}
    virtual QString getName();
    virtual QString getModelName();
    virtual bool isValid();

    virtual void getCopyModelsIncludesList(QSet<QString> &list);
    virtual void getCopyItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getCopyModelsDefinition();
    virtual QString getCopyModelsHierachy();
    virtual QString getCopyModelAddingCommand();
    virtual QString getCopyComputeBeginning(int rank, QString resultName = "");
    virtual QString getCopyComputeItemsCreations(QString resultName = "");

};

#endif // COPYRESULTMODEL_H
