#ifndef INGROUPMODEL_H
#define INGROUPMODEL_H

#include "models/abstractinmodel.h"

class INGroupModel : public AbstractInModel
{
public:
    INGroupModel();

    virtual AbstractInModel::ModelType getModelType() {return AbstractInModel::M_Group_IN;}
    virtual QString getName();
    virtual QString getModelName();

    virtual void getInModelsIncludesList(QSet<QString> &list);
    virtual void getInItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getInModelsDefinition();
    virtual QString getInModelsHierachy();
    virtual QString getInModelAddingCommand();
    virtual QString getInComputeBeginning(QString resultDef = "", QString useCopy = "");
    virtual QString getInComputeLoops(int nbIndent);
    virtual QString getInModelDoc(int nbIndent = 0);

};

#endif // INGROUPMODEL_H
