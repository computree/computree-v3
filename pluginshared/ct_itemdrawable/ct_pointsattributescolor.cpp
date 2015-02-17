#include "ct_pointsattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_StandardPointsAttributesColorDrawManager CT_PointsAttributesColor::PAC_DRAW_MANAGER;

CT_PointsAttributesColor::CT_PointsAttributesColor() : CT_AbstractPointsAttributes(), CT_AttributesColor()
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(const CT_OutAbstractSingularItemModel *model,
                                                   const CT_AbstractResult *result,
                                                   CT_PCIR pcir,
                                                   bool withAlphaInformation) : CT_AbstractPointsAttributes(model,
                                                                                                            result,
                                                                                                            pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(const CT_OutAbstractSingularItemModel *model,
                                                   const CT_AbstractResult *result,
                                                   CT_PCIR pcir,
                                                   CT_AbstractColorCloud *cc) : CT_AbstractPointsAttributes(model,
                                                                                                            result,
                                                                                                            pcir), CT_AttributesColor(cc)
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(const QString &modelName,
                                                   const CT_AbstractResult *result,
                                                   CT_PCIR pcir,
                                                   bool withAlphaInformation) : CT_AbstractPointsAttributes(modelName,
                                                                                                            result,
                                                                                                            pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(const QString &modelName,
                                                   const CT_AbstractResult *result,
                                                   CT_PCIR pcir,
                                                   CT_AbstractColorCloud *cc) : CT_AbstractPointsAttributes(modelName,
                                                                                                            result,
                                                                                                            pcir), CT_AttributesColor(cc)
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

QString CT_PointsAttributesColor::getType() const
{
    return staticGetType();
}

QString CT_PointsAttributesColor::staticGetType()
{
    return CT_AbstractPointsAttributes::staticGetType() + "/CT_PointsAttributesColor";
}

CT_AbstractItemDrawable* CT_PointsAttributesColor::copy(const CT_OutAbstractItemModel *model,
                                                        const CT_AbstractResult *result,
                                                        CT_ResultCopyModeList copyModeList)
{
    CT_AbstractColorCloud* colorCloud = getColorCloud();

    if (colorCloud == NULL)
        return new CT_PointsAttributesColor((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered(), (CT_AbstractColorCloud*) NULL);
    else
        return new CT_PointsAttributesColor((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered(), dynamic_cast<CT_AbstractColorCloud*>(colorCloud->copy()));
}
