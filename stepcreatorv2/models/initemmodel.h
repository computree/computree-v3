#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abstractinmodel.h"

class INItemModel : public AbstractInModel
{
public:
    INItemModel();

    virtual AbstractInModel::ModelType getModelType() {return AbstractInModel::M_Item_IN;}
    virtual QString getName();
    virtual QString getModelName();
    QString getItemType();

    virtual void getInModelsIncludesList(QSet<QString> &list);
    virtual void getInItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeBeginning(QString resultDef = "", QString useCopy = "");
    virtual QString getInComputeLoops(int nbIndent);
};

#endif // INITEMMODEL_H
