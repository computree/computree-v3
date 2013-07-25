#ifndef COPYGROUPMODEL_H
#define COPYGROUPMODEL_H

#include "models/abstractcopymodel.h"

class COPYGroupModel : public AbstractCopyModel
{
public:
    COPYGroupModel();

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Group_COPY;}
    virtual QString getName();
    virtual QString getModelName();

    virtual void getCopyModelsIncludesList(QSet<QString> &list);
    virtual void getCopyItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getCopyModelsDefinition();
    virtual QString getCopyModelsHierachy();
    virtual QString getCopyModelAddingCommand();
    virtual QString getCopyComputeBeginning(int rank = 0, QString resultName = "");
    virtual QString getCopyComputeItemsCreations(QString resultName = "");

};

#endif // COPYGROUPMODEL_H
