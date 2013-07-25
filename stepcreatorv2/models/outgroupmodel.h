#ifndef OUTGROUPMODEL_H
#define OUTGROUPMODEL_H

#include "models/abstractoutmodel.h"

class OUTGroupModel : public AbstractOutModel
{
public:
    OUTGroupModel();

    virtual AbstractOutModel::ModelType getModelType() {return AbstractOutModel::M_Group_OUT;}
    virtual QString getName();
    virtual QString getModelName();

    virtual void getOutModelsIncludesList(QSet<QString> &list);
    virtual void getOutItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getOutModelsDefinition();
    virtual QString getOutModelsHierachy();
    virtual QString getOutModelAddingCommand();
    virtual QString getOutComputeBeginning(QString resultDef = "", QString useCopy = "");
    virtual QString getOutComputeLoops(int nbIndent);

};

#endif // OUTGROUPMODEL_H
