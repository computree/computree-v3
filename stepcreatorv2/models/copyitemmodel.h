#ifndef COPYITEMMODEL_H
#define COPYITEMMODEL_H

#include "models/abstractcopymodel.h"

class COPYItemModel : public AbstractCopyModel
{
public:
    COPYItemModel();

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Item_COPY;}
    virtual QString getName();
    virtual QString getModelName();
    QString getItemType();

    virtual void getCopyModelsIncludesList(QSet<QString> &list);
    virtual void getCopyItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getCopyModelsDefinition();
    virtual QString getCopyModelsHierachy();
    virtual QString getCopyModelAddingCommand();
    virtual QString getCopyComputeBeginning(int rank = 0, QString resultName = "");
    virtual QString getCopyComputeItemsCreations(QString resultName = "");
};

#endif // OUITEMMODEL_H
