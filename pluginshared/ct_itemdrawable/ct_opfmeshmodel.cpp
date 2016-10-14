#include "ct_opfmeshmodel.h"

CT_OPFMeshModel::CT_OPFMeshModel()
{
    defaultOpfMeshModelConstructorInitialization();
}

CT_OPFMeshModel::CT_OPFMeshModel(const CT_OutAbstractSingularItemModel *model,
                                 const CT_AbstractResult *result) : CT_MeshModel(model, result)
{
    defaultOpfMeshModelConstructorInitialization();
}

CT_OPFMeshModel::CT_OPFMeshModel(const CT_OutAbstractSingularItemModel *model,
                                 const CT_AbstractResult *result,
                                 CT_Mesh *mesh) : CT_MeshModel(model, result, mesh)
{
    defaultOpfMeshModelConstructorInitialization();
}

CT_OPFMeshModel::CT_OPFMeshModel(const QString &modelName,
                                 const CT_AbstractResult *result) : CT_MeshModel(modelName, result)
{
    defaultOpfMeshModelConstructorInitialization();
}

CT_OPFMeshModel::CT_OPFMeshModel(const QString &modelName,
                                 const CT_AbstractResult *result,
                                 CT_Mesh *mesh) : CT_MeshModel(modelName, result, mesh)
{
    defaultOpfMeshModelConstructorInitialization();
}

QString CT_OPFMeshModel::name() const
{
    return tr("OPF mesh model");
}

void CT_OPFMeshModel::setDUp(double dUp)
{
    m_dUp = dUp;
}

void CT_OPFMeshModel::setDDown(double dDown)
{
    m_dDown = dDown;
}

double CT_OPFMeshModel::dUp() const
{
    return m_dUp;
}

double CT_OPFMeshModel::dDown() const
{
    return m_dDown;
}

void CT_OPFMeshModel::defaultOpfMeshModelConstructorInitialization()
{
    m_dUp = 1.0;
    m_dDown = 1.0;
}
