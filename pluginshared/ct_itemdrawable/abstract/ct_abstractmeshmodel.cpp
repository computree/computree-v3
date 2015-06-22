#include "ct_abstractmeshmodel.h"

#include "ct_tools/pcl/ct_pcltools.h"

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

QString CT_AbstractMeshModel::getType() const
{
    return staticGetType();
}

QString CT_AbstractMeshModel::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_AbstractMeshModel";
}

#ifdef USE_PCL
boost::shared_ptr<pcl::PointCloud<CT_PointData> > CT_AbstractMeshModel::getPCLCloud() const
{
    return CT_PCLTools::staticConvertToPCLCloud(this);
}
#endif
