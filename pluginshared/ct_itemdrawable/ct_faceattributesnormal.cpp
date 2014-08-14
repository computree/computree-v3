#include "ct_faceattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_FaceAttributesNormal::CT_FaceAttributesNormal() : CT_AbstractFaceAttributes(), CT_AttributesNormal()
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(const CT_OutAbstractItemModel *model,
                                                 const CT_AbstractResult *result,
                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir) : CT_AbstractFaceAttributes(model,
                                                                                                                                                      result,
                                                                                                                                                      pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(const CT_OutAbstractItemModel *model,
                                                 const CT_AbstractResult *result,
                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                 CT_AbstractNormalCloud *nc) : CT_AbstractFaceAttributes(model,
                                                                                                         result,
                                                                                                         pcir), CT_AttributesNormal(nc)
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(const QString &modelName,
                                                 const CT_AbstractResult *result,
                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir) : CT_AbstractFaceAttributes(modelName,
                                                                                                                                                      result,
                                                                                                                                                      pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(const QString &modelName,
                                                 const CT_AbstractResult *result,
                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                 CT_AbstractNormalCloud *nc) : CT_AbstractFaceAttributes(modelName,
                                                                                                         result,
                                                                                                         pcir), CT_AttributesNormal(nc)
{
}


QString CT_FaceAttributesNormal::getType() const
{
    return staticGetType();
}

QString CT_FaceAttributesNormal::staticGetType()
{
    return CT_AbstractFaceAttributes::staticGetType() + "/CT_FaceAttributesNormal";
}

CT_AbstractItemDrawable* CT_FaceAttributesNormal::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_AbstractNormalCloud* normalCloud = getNormalCloud();

    if (normalCloud == NULL)
        return new CT_FaceAttributesNormal(model, result, getFaceCloudIndexRegistered(), (CT_AbstractNormalCloud*) NULL);
    else
        return new CT_FaceAttributesNormal(model, result, getFaceCloudIndexRegistered(), dynamic_cast<CT_AbstractNormalCloud*>(normalCloud->copy()));
}
