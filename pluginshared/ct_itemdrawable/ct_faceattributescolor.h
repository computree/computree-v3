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
                           CT_FCIR pcir);

    CT_FaceAttributesColor(const CT_OutAbstractSingularItemModel *model,
                           const CT_AbstractResult *result,
                           CT_FCIR pcir,
                           CT_AbstractColorCloud *cc);

    CT_FaceAttributesColor(const QString &modelName,
                           const CT_AbstractResult *result,
                           CT_FCIR pcir);

    CT_FaceAttributesColor(const QString &modelName,
                           const CT_AbstractResult *result,
                           CT_FCIR pcir,
                           CT_AbstractColorCloud *cc);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    size_t attributesSize() const { return CT_AttributesColor::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    CT_DEFAULT_IA_BEGIN(CT_FaceAttributesColor)
    CT_DEFAULT_IA_V2(CT_FaceAttributesColor, CT_AbstractCategory::staticInitDataSize(), &CT_FaceAttributesColor::attributesSize, QObject::tr("Taille"))
    CT_DEFAULT_IA_END(CT_FaceAttributesColor)
};

#endif // CT_FACEATTRIBUTESCOLOR_H
