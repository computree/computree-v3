#include "ct_abstractmeshmodel.h"

CT_AbstractMeshModel::CT_AbstractMeshModel() : CT_AbstractSingularItemDrawable()
{
}

CT_AbstractMeshModel::CT_AbstractMeshModel(const CT_OutAbstractSingularItemModel *model,
                                           const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{
}

CT_AbstractMeshModel::CT_AbstractMeshModel(const QString &modelName,
                                           const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{
}

QString CT_AbstractMeshModel::name() const
{
    return tr("Mesh model");
}
