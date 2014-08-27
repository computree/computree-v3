#ifndef DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
#define DM_ATTRIBUTESBUILDINGCOLLECTIONT_H

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/tools/iterator/ct_resultiterator.h"

/**
 * @brief Recursively build a collection of attributes available from a step.
 */
template<typename Type>
class DM_AttributesBuildingCollectionT
{
public:
    DM_AttributesBuildingCollectionT();

    bool buildFrom(CT_VirtualAbstractStep *step);

    const QList<Type*>& attributesCollection() const;

private:
    QList<Type*>   m_collection;

    void recursiveBuildAttributesFromStep(CT_VirtualAbstractStep *step);
    void recursiveBuildAttributesFromModels(QList<CT_AbstractModel *> models);
};

#include "tools/attributes/dm_attributesbuildingcollectiont.hpp"

#endif // DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
