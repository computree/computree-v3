#ifndef CT_OPFMESHMODEL_H
#define CT_OPFMESHMODEL_H

#include "ct_itemdrawable/ct_meshmodel.h"

class PLUGINSHAREDSHARED_EXPORT CT_OPFMeshModel : public CT_MeshModel
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_OPFMeshModel, CT_MeshModel, OPF mesh model)

public:
    CT_OPFMeshModel();
    CT_OPFMeshModel(const CT_OutAbstractSingularItemModel *model,
                    const CT_AbstractResult *result);
    CT_OPFMeshModel(const CT_OutAbstractSingularItemModel *model,
                    const CT_AbstractResult *result,
                    CT_Mesh *mesh);

    CT_OPFMeshModel(const QString &modelName,
                    const CT_AbstractResult *result);
    CT_OPFMeshModel(const QString &modelName,
                    const CT_AbstractResult *result,
                    CT_Mesh *mesh);

    void setDUp(double dUp);
    void setDDown(double dDown);

    double dUp() const;
    double dDown() const;

private:
    double m_dUp;
    double m_dDown;

    void defaultOpfMeshModelConstructorInitialization();
};

#endif // CT_OPFMESHMODEL_H
