#include "ct_edgeattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal() : CT_AbstractEdgeAttributes(), CT_AttributesNormal()
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                                                 const CT_AbstractResult *result,
                                                 CT_ECIR pcir) : CT_AbstractEdgeAttributes(model,
                                                                                                                                                      result,
                                                                                                                                                      pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                                                 const CT_AbstractResult *result,
                                                 CT_ECIR pcir,
                                                 CT_AbstractNormalCloud *nc) : CT_AbstractEdgeAttributes(model,
                                                                                                         result,
                                                                                                         pcir), CT_AttributesNormal(nc)
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(const QString &modelName,
                                                 const CT_AbstractResult *result,
                                                 CT_ECIR pcir) : CT_AbstractEdgeAttributes(modelName,
                                                                                                                                                      result,
                                                                                                                                                      pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(const QString &modelName,
                                                 const CT_AbstractResult *result,
                                                 CT_ECIR pcir,
                                                 CT_AbstractNormalCloud *nc) : CT_AbstractEdgeAttributes(modelName,
                                                                                                         result,
                                                                                                         pcir), CT_AttributesNormal(nc)
{
}

QString CT_EdgeAttributesNormal::getType() const
{
    return staticGetType();
}

QString CT_EdgeAttributesNormal::staticGetType()
{
    return CT_AbstractEdgeAttributes::staticGetType() + "/CT_EdgeAttributesNormal";
}

CT_AbstractItemDrawable* CT_EdgeAttributesNormal::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_AbstractNormalCloud* normalCloud = getNormalCloud();

    if (normalCloud == NULL)
        return new CT_EdgeAttributesNormal((const CT_OutAbstractSingularItemModel *)model, result, getEdgeCloudIndexRegistered(), (CT_AbstractNormalCloud*) NULL);
    else
        return new CT_EdgeAttributesNormal((const CT_OutAbstractSingularItemModel *)model, result, getEdgeCloudIndexRegistered(), dynamic_cast<CT_AbstractNormalCloud*>(normalCloud->copy()));
}
