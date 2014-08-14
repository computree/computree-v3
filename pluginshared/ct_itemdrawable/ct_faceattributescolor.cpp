#include "ct_faceattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_FaceAttributesColor::CT_FaceAttributesColor() : CT_AbstractFaceAttributes(), CT_AttributesColor()
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(const CT_OutAbstractItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                               bool withAlphaInformation) : CT_AbstractFaceAttributes(model,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(const CT_OutAbstractItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                               CT_AbstractColorCloud *cc) : CT_AbstractFaceAttributes(model,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(cc)
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(const QString &modelName,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                               bool withAlphaInformation) : CT_AbstractFaceAttributes(modelName,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size(), withAlphaInformation))
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(const QString &modelName,
                                               const CT_AbstractResult *result,
                                               CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                               CT_AbstractColorCloud *cc) : CT_AbstractFaceAttributes(modelName,
                                                                                                      result,
                                                                                                      pcir), CT_AttributesColor(cc)
{
}

QString CT_FaceAttributesColor::getType() const
{
    return staticGetType();
}

QString CT_FaceAttributesColor::staticGetType()
{
    return CT_AbstractFaceAttributes::staticGetType() + "/CT_FaceAttributesColor";
}

CT_AbstractItemDrawable* CT_FaceAttributesColor::copy(const CT_OutAbstractItemModel *model,
                                                      const CT_AbstractResult *result,
                                                      CT_ResultCopyModeList copyModeList)
{
    CT_AbstractColorCloud* colorCloud = getColorCloud();

    if (colorCloud == NULL)
        return new CT_FaceAttributesColor(model, result, getFaceCloudIndexRegistered(), (CT_AbstractColorCloud*) NULL);
    else
        return new CT_FaceAttributesColor(model, result, getFaceCloudIndexRegistered(), dynamic_cast<CT_AbstractColorCloud*>(colorCloud->copy()));
}
