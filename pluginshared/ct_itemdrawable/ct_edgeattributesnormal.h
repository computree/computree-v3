#ifndef CT_EDGEATTRIBUTESNORMAL_H
#define CT_EDGEATTRIBUTESNORMAL_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class PLUGINSHAREDSHARED_EXPORT CT_EdgeAttributesNormal : public CT_AbstractEdgeAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_EdgeAttributesNormal, CT_AbstractEdgeAttributes)

public:
    CT_EdgeAttributesNormal();

    CT_EdgeAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            CT_ECIR pcir);

    CT_EdgeAttributesNormal(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            CT_ECIR pcir,
                            CT_AbstractNormalCloud *nc);

    CT_EdgeAttributesNormal(const QString &modelName,
                            const CT_AbstractResult *result,
                            CT_ECIR pcir);


    CT_EdgeAttributesNormal(const QString &modelName,
                            const CT_AbstractResult *result,
                            CT_ECIR pcir,
                            CT_AbstractNormalCloud *nc);

    size_t attributesSize() const { return CT_AttributesNormal::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_EDGEATTRIBUTESNORMAL_H
