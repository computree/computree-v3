#ifndef COPYRESULTMODEL_H
#define COPYRESULTMODEL_H

#include "models/abstractcopymodel.h"
#include "models/inresultmodel.h"

class COPYResultModel : public AbstractCopyModel
{
public:
    COPYResultModel();
    void init(INResultModel *inModel);

    void init(QString alias, QString name, QString desc);

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Result_COPY;}
    virtual QString getName();
    virtual QString getModelName();
    virtual bool isValid();
    virtual QString getAutoRenameName();

    virtual void getActionsIncludes(QSet<QString> &list);
    virtual QString getCopyModelsDefinitions(QString actionName = "");

};

#endif // COPYRESULTMODEL_H
