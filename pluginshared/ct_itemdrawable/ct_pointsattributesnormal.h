#ifndef CT_POINTSATTRIBUTESNORMAL_H
#define CT_POINTSATTRIBUTESNORMAL_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class PLUGINSHAREDSHARED_EXPORT CT_PointsAttributesNormal : public CT_AbstractPointsAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointsAttributesNormal, CT_AbstractPointsAttributes, Normal point attributes)

public:
    CT_PointsAttributesNormal();

    CT_PointsAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_PCIR pcir);

    CT_PointsAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_PCIR pcir,
                              CT_AbstractNormalCloud *nc);

    CT_PointsAttributesNormal(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_PCIR pcir);

    CT_PointsAttributesNormal(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_PCIR pcir,
                              CT_AbstractNormalCloud *nc);

    size_t attributesSize() const { return CT_AttributesNormal::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    static CT_StandardPointsAttributesNormalDrawManager PAN_DRAW_MANAGER;
};

#endif // CT_POINTSATTRIBUTESNORMAL_H
