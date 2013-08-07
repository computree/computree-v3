#ifndef COPYGROUPMODEL_H
#define COPYGROUPMODEL_H

#include "models/ingroupmodel.h"
#include "models/abstractcopymodel.h"

class COPYGroupModel : public AbstractCopyModel
{
public:
    COPYGroupModel();
    void init(INGroupModel *inModel);
    void init(QString alias, QString name, QString desc);

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Group_COPY;}
    virtual QString getName();
    virtual QString getModelName();

    virtual void getActionsIncludes(QSet<QString> &list);
    virtual QString getCopyModelsDefinitions(QString actionName = "");
};

#endif // COPYGROUPMODEL_H
