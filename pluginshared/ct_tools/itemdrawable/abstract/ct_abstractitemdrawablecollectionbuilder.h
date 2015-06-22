#ifndef CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H
#define CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H

#include "ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchystep.h"

class CT_VirtualAbstractStep;

/**
 * @brief A class that define the interface for a builder that search other itemdrawable to export
 *        from a step.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawableCollectionBuilder
{
public:
    CT_AbstractItemDrawableCollectionBuilder();
    virtual ~CT_AbstractItemDrawableCollectionBuilder() {}

    /**
     * @brief Search elements from step passed in parameter.
     */
    virtual bool buildFrom(const CT_VirtualAbstractStep *step, bool findOnlyModels) = 0;

    /**
     * @brief Returns the list of results sorted by step
     */
    virtual const QList<CT_ItemDrawableCollectionHierarchyStep>& collection() const = 0;
};

#endif // CT_ABSTRACTITEMDRAWABLECOLLECTIONBUILDER_H
