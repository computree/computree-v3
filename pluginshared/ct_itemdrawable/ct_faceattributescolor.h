#ifndef CT_FACEATTRIBUTESCOLOR_H
#define CT_FACEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class PLUGINSHAREDSHARED_EXPORT CT_FaceAttributesColor : public CT_AbstractFaceAttributes, public CT_AttributesColor
{
    Q_OBJECT

public:
    CT_FaceAttributesColor();

    CT_FaceAttributesColor(const CT_OutAbstractSingularItemModel *model,
                           const CT_AbstractResult *result,
                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                           bool withAlphaInformation);

    CT_FaceAttributesColor(const CT_OutAbstractSingularItemModel *model,
                           const CT_AbstractResult *result,
                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                           CT_AbstractColorCloud *cc);

    CT_FaceAttributesColor(const QString &modelName,
                           const CT_AbstractResult *result,
                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                           bool withAlphaInformation);

    CT_FaceAttributesColor(const QString &modelName,
                           const CT_AbstractResult *result,
                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                           CT_AbstractColorCloud *cc);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    size_t attributesSize() const { return CT_AttributesColor::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_FACEATTRIBUTESCOLOR_H
