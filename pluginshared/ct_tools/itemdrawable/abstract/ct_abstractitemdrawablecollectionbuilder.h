#ifndef CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H
#define CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H

#include "ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchystep.h"

class CT_VirtualAbstractStep;

class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawableCollectionBuilder
{
public:
    CT_AbstractItemDrawableCollectionBuilder();
    virtual ~CT_AbstractItemDrawableCollectionBuilder() {}

    virtual bool buildFrom(const CT_VirtualAbstractStep *step) = 0;

    virtual const QList<CT_ItemDrawableCollectionHierarchyStep>& collection() const = 0;
};

#endif // CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H
