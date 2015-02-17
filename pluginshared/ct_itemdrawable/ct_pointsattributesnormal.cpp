#include "ct_pointsattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_StandardPointsAttributesNormalDrawManager CT_PointsAttributesNormal::PAN_DRAW_MANAGER;

CT_PointsAttributesNormal::CT_PointsAttributesNormal() : CT_AbstractPointsAttributes(), CT_AttributesNormal()
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_PCIR pcir) : CT_AbstractPointsAttributes(model,
                                                                                                                                                             result,
                                                                                                                                                             pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_PCIR pcir,
                                                     CT_AbstractNormalCloud *nc) : CT_AbstractPointsAttributes(model,
                                                                                                               result,
                                                                                                               pcir), CT_AttributesNormal(nc)
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}


CT_PointsAttributesNormal::CT_PointsAttributesNormal(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_PCIR pcir) : CT_AbstractPointsAttributes(modelName,
                                                                                                                                                             result,
                                                                                                                                                             pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_PCIR pcir,
                                                     CT_AbstractNormalCloud *nc) : CT_AbstractPointsAttributes(modelName,
                                                                                                               result,
                                                                                                               pcir), CT_AttributesNormal(nc)
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

QString CT_PointsAttributesNormal::getType() const
{
    return staticGetType();
}

QString CT_PointsAttributesNormal::staticGetType()
{
    return CT_AbstractPointsAttributes::staticGetType() + "/CT_PointsAttributesNormal";
}

CT_AbstractItemDrawable* CT_PointsAttributesNormal::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_AbstractNormalCloud* normalCloud = getNormalCloud();

    if (normalCloud == NULL)
        return new CT_PointsAttributesNormal((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered(), (CT_AbstractNormalCloud*) NULL);
    else
        return new CT_PointsAttributesNormal((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered(), dynamic_cast<CT_AbstractNormalCloud*>(normalCloud->copy()));
}
