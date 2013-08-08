#ifndef OUTITEMMODEL_H
#define OUTITEMMODEL_H

#include "models/abstractoutmodel.h"

class OUTItemModel : public AbstractOutModel
{
public:
    OUTItemModel();

    virtual AbstractOutModel::ModelType getModelType() {return AbstractOutModel::M_Item_OUT;}
    virtual QString getName();
    virtual QString getModelName();
    QString getItemType();

    virtual void getOutModelsIncludesList(QSet<QString> &list);
    virtual void getOutItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getOutModelsDefinition();
    virtual QString getOutModelsHierachy();
    virtual QString getOutModelAddingCommand();
    virtual QString getOutComputeBeginning(int rank = 0, QString resultName = "");
    virtual QString getOutComputeItemsCreations(QString resultName = "");
    virtual QString getOutModelDoc(int nbIndent = 0);
};

#endif // OUITEMMODEL_H
