#ifndef DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP
#define DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP

#include "tools/attributes/dm_attributesbuildingcollectiont.h"

#include "ct_result/abstract/ct_abstractresult.h"

template<typename Type>
DM_AttributesBuildingCollectionT<Type>::DM_AttributesBuildingCollectionT()
{
}

template<typename Type>
bool DM_AttributesBuildingCollectionT<Type>::buildFrom(CT_VirtualAbstractStep *step)
{
    m_collection.clear();

    if(step == NULL)
        return false;

    recursiveBuildAttributesFromStep(step);

    return true;
}

template<typename Type>
const QList<Type*>& DM_AttributesBuildingCollectionT<Type>::attributesCollection() const
{
    return m_collection;
}

template<typename Type>
void DM_AttributesBuildingCollectionT<Type>::recursiveBuildAttributesFromStep(CT_VirtualAbstractStep *step)
{
    QList<CT_ResultGroup*> results = step->getResults();
    QListIterator<CT_ResultGroup*> it(results);

    while(it.hasNext())
        recursiveBuildAttributesFromModels(it.next()->model()->childrens());

    QList<CT_VirtualAbstractStep*> steps = step->getStepChildList();

    while(!steps.isEmpty())
        recursiveBuildAttributesFromStep(steps.takeFirst());
}

template<typename Type>
void DM_AttributesBuildingCollectionT<Type>::recursiveBuildAttributesFromModels(QList<CT_AbstractModel*> models)
{
    QListIterator<CT_AbstractModel*> itM(models);

    while(itM.hasNext())
    {
        CT_AbstractModel *model = itM.next();

        if((dynamic_cast<Type*>(((CT_OutAbstractModel*)model)->item()) != NULL)
                && (((CT_OutAbstractModel*)model)->result() != NULL))
        {
            CT_ResultIterator it((CT_ResultGroup*)((CT_OutAbstractModel*)model)->result(), model);

            while(it.hasNext())
                m_collection.append((Type*)it.next());
        }

        recursiveBuildAttributesFromModels(model->childrens());
    }
}


#endif // DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP
