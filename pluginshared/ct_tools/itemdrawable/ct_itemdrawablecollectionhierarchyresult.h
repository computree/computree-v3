#ifndef CT_ITEMDRAWABLECOLLECTIONHIERARCHYRESULT_H
#define CT_ITEMDRAWABLECOLLECTIONHIERARCHYRESULT_H

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

/**
 * @brief A collection of itemdrawable in a result that can be exported
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableCollectionHierarchyResult
{
public:
    CT_ItemDrawableCollectionHierarchyResult() { result = NULL; }

    CT_ResultGroup                              *result;
    QList<CT_AbstractSingularItemDrawable*>     collection;
};

#endif // CT_ITEMDRAWABLECOLLECTIONHIERARCHYRESULT_H
