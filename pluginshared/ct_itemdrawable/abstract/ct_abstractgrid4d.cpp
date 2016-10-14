#include "ct_abstractgrid4d.h"

CT_AbstractGrid4D::CT_AbstractGrid4D() : CT_AbstractItemDrawableWithoutPointCloud()
{
}

CT_AbstractGrid4D::CT_AbstractGrid4D(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
}

CT_AbstractGrid4D::CT_AbstractGrid4D(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
}

CT_AbstractGrid4D::~CT_AbstractGrid4D()
{
}

QString CT_AbstractGrid4D::name() const
{
    return tr("4D grid");
}
