#ifndef CT_FACEATTRIBUTESNORMAL_H
#define CT_FACEATTRIBUTESNORMAL_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class PLUGINSHAREDSHARED_EXPORT CT_FaceAttributesNormal : public CT_AbstractFaceAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_FaceAttributesNormal, CT_AbstractFaceAttributes, Face normal attributes)

public:
    CT_FaceAttributesNormal();

    CT_FaceAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_FCIR pcir);

    CT_FaceAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_FCIR pcir,
                              CT_AbstractNormalCloud *nc);

    CT_FaceAttributesNormal(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_FCIR pcir);

    CT_FaceAttributesNormal(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_FCIR pcir,
                              CT_AbstractNormalCloud *nc);

    size_t attributesSize() const { return CT_AttributesNormal::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_FACEATTRIBUTESNORMAL_H
