#ifndef CT_ABSTRACTEDGEATTRIBUTESSCALAR_H
#define CT_ABSTRACTEDGEATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractEdgeAttributesScalar : public CT_AbstractEdgeAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractEdgeAttributesScalar, CT_AbstractEdgeAttributes)

public:
    CT_AbstractEdgeAttributesScalar();
    CT_AbstractEdgeAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                    CT_AbstractResult *result,
                                    CT_ECIR cir);

    CT_AbstractEdgeAttributesScalar(const QString &modelName,
                                    CT_AbstractResult *result,
                                    CT_ECIR cir);

    virtual double dMin() const = 0;
    virtual double dMax() const = 0;

    virtual double dValueAt(const size_t &index) const = 0;

    virtual size_t attributesSize() const = 0;
};

#endif // CT_ABSTRACTEDGEATTRIBUTESSCALAR_H
