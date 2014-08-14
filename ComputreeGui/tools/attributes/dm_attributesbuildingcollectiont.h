#ifndef DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
#define DM_ATTRIBUTESBUILDINGCOLLECTIONT_H

#include "interfaces.h"

/**
 * @brief Recursively build a collection of attributes available from a step.
 */
template<typename Type>
class DM_AttributesBuildingCollectionT
{
public:
    DM_AttributesBuildingCollectionT();

    bool buildFrom(Step *step);

    const QList<Type*>& attributesCollection() const;

private:
    QList<Type*>   m_collection;

    void recursiveBuildAttributesFromStep(Step *step);
    void recursiveBuildAttributesFromModels(QList<IItemModel*> models);
};

#include "tools/attributes/dm_attributesbuildingcollectiont.hpp"

#endif // DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
