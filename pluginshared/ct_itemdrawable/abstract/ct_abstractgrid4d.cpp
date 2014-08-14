#include "ct_abstractgrid4d.h"

CT_AbstractGrid4D::CT_AbstractGrid4D() : CT_AbstractItemDrawableWithoutPointCloud()
{
}

CT_AbstractGrid4D::CT_AbstractGrid4D(const CT_OutAbstractItemModel *model, CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
}

CT_AbstractGrid4D::CT_AbstractGrid4D(const QString &modelName, CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
}

CT_AbstractGrid4D::~CT_AbstractGrid4D()
{
}
