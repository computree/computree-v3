#ifndef OUTRESULTMODEL_H
#define OUTRESULTMODEL_H

#include "models/abstractoutmodel.h"

class OUTResultModel : public AbstractOutModel
{
public:
    OUTResultModel();

    virtual AbstractOutModel::ModelType getModelType() {return AbstractOutModel::M_Result_OUT;}
    virtual QString getName();
    virtual QString getModelName();
    virtual bool isValid();

    virtual void getOutModelsIncludesList(QSet<QString> &list);
    virtual void getOutItemsTypesIncludesList(QSet<QString> &list);
    virtual QString getOutModelsDefinition();
    virtual QString getOutModelsHierachy();
    virtual QString getOutModelAddingCommand();
    virtual QString getOutComputeBeginning(QString resultDef = "", QString useCopy = "");
    virtual QString getOutComputeLoops(int nbIndent);

};

#endif // OUTRESULTMODEL_H
