#ifndef CT_POINTSATTRIBUTESCOLOR_H
#define CT_POINTSATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.h"
#include "ct_attributes/ct_attributescolor.h"

class PLUGINSHAREDSHARED_EXPORT CT_PointsAttributesColor : public CT_AbstractPointsAttributes, public CT_AttributesColor
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointsAttributesColor, CT_AbstractPointsAttributes)

public:
    CT_PointsAttributesColor();

    CT_PointsAttributesColor(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_PCIR pcir);

    CT_PointsAttributesColor(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_PCIR pcir,
                             CT_AbstractColorCloud *cc);

    CT_PointsAttributesColor(const QString &modelName,
                             const CT_AbstractResult *result,
                             CT_PCIR pcir);

    CT_PointsAttributesColor(const QString &modelName,
                             const CT_AbstractResult *result,
                             CT_PCIR pcir,
                             CT_AbstractColorCloud *cc);

    size_t attributesSize() const { return CT_AttributesColor::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    static CT_StandardPointsAttributesColorDrawManager PAC_DRAW_MANAGER;
};

#endif // CT_POINTSATTRIBUTESCOLOR_H
