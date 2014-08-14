#ifndef DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP
#define DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP

#include "tools/attributes/dm_attributesbuildingcollectiont.h"

template<typename Type>
DM_AttributesBuildingCollectionT<Type>::DM_AttributesBuildingCollectionT()
{
}

template<typename Type>
bool DM_AttributesBuildingCollectionT<Type>::buildFrom(Step *step)
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
void DM_AttributesBuildingCollectionT<Type>::recursiveBuildAttributesFromStep(Step *step)
{
    QList<Result*> results = step->getResults();
    QListIterator<Result*> it(results);

    while(it.hasNext())
        recursiveBuildAttributesFromModels(it.next()->getModel()->getRootItemModel());

    QList<Step*> steps = step->getStepChildList();

    while(!steps.isEmpty())
        recursiveBuildAttributesFromStep(steps.takeFirst());
}

template<typename Type>
void DM_AttributesBuildingCollectionT<Type>::recursiveBuildAttributesFromModels(QList<IItemModel*> models)
{
    QListIterator<IItemModel*> itM(models);

    while(itM.hasNext())
    {
        IItemModel *model = itM.next();

        if(dynamic_cast<Type*>(model->itemDrawable()) != NULL)
        {
            if((model->result() != NULL)
                    && (model->result()->recursiveBeginIterateItemDrawableWithModel(*model) > 0))
            {
                ItemDrawable *item;

                while((item = model->result()->recursiveNextItemDrawable()) != NULL)
                {
                    m_collection.append(dynamic_cast<Type*>(item));
                }
            }
        }

        recursiveBuildAttributesFromModels(model->getChildrenItemModel());
    }
}


#endif // DM_ATTRIBUTESBUILDINGCOLLECTIONT_HPP
