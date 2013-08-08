#ifndef COPYITEMMODEL_H
#define COPYITEMMODEL_H

#include "models/initemmodel.h"
#include "models/abstractcopymodel.h"

class COPYItemModel : public AbstractCopyModel
{
public:
    COPYItemModel();
    void init(INItemModel *inModel);
    void init(QString itemType, QString alias, QString name, QString desc);

    virtual AbstractCopyModel::ModelType getModelType() {return AbstractCopyModel::M_Item_COPY;}
    virtual QString getName();
    virtual QString getModelName();
    QString getItemType();

    virtual void getActionsIncludes(QSet<QString> &list);
    virtual QString getCopyModelsDefinitions(QString actionName = "");
    virtual QString getCopyComputeLoops(int nbIndent, QString resultName = "");
    virtual QString getCopyModelDoc(int nbIndent = 0);

};

#endif // OUITEMMODEL_H
