#ifndef CT_EDGEATTRIBUTESCOLOR_H
#define CT_EDGEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class PLUGINSHAREDSHARED_EXPORT CT_EdgeAttributesColor : public CT_AbstractEdgeAttributes, public CT_AttributesColor
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_EdgeAttributesColor, CT_AbstractEdgeAttributes)

public:
    CT_EdgeAttributesColor();

    CT_EdgeAttributesColor(const CT_OutAbstractSingularItemModel *model,
                           const CT_AbstractResult *result,
                             CT_ECIR pcir);

    CT_EdgeAttributesColor(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_ECIR pcir,
                             CT_AbstractColorCloud *cc);


    CT_EdgeAttributesColor(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_ECIR pcir);

    CT_EdgeAttributesColor(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_ECIR pcir,
                     CT_AbstractColorCloud *cc);

    virtual QString name() const;


    size_t attributesSize() const { return CT_AttributesColor::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_EDGEATTRIBUTESCOLOR_H
