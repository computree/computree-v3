#ifndef CT_STANDARDGLOBALCLOUDITEMDATAVALUEDELEGATET_H
#define CT_STANDARDGLOBALCLOUDITEMDATAVALUEDELEGATET_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdatavaluedelegate.h"

template<class ItemDrawableClass, class CloudType, class CloudIndexType>
class CT_StandardGlobalCloudItemDataValueDelegateT : public CT_AbstractItemDataValueDelegate
{
public:
    typedef const CloudType* (ItemDrawableClass::*funcCloud)() const;
    typedef const CloudIndexType* (ItemDrawableClass::*funcCloudIndex)() const;

    CT_StandardGlobalCloudItemDataValueDelegateT(funcCloud getCloudFunction, funcCloudIndex getCloudIndexFunction, const int &index) : CT_AbstractItemDataValueDelegate()
    {
        m_cloudFunction = getCloudFunction;
        m_cloudIndexFunction = getCloudIndexFunction;
        m_index = index;
    }

    IItemDataValue::ItemDataValueType dataType() const
    {
        return (IItemDataValue::ItemDataValueType)CT_StandardItemDataValue::staticDataTypeToCloudType<CloudType>();
    }

    bool value(const CT_AbstractItemDrawable *item, IItemDataValue* valueToSet) const
    {
        const ItemDrawableClass *dItem = dynamic_cast<const ItemDrawableClass*>(item);

        const CloudType *cloud = (dItem->*m_cloudFunction)();
        const CloudIndexType *cloudIndex = (dItem->*m_cloudIndexFunction)();

        ((CT_StandardItemDataValue*)valueToSet)->setValue(cloud, cloudIndex, m_index);
        return true;
    }

    CT_AbstractItemDataValueDelegate* copy() const
    {
        return new CT_StandardGlobalCloudItemDataValueDelegateT<ItemDrawableClass, CloudType, CloudIndexType>(this->m_cloudFunction, this->m_cloudIndexFunction, this->m_index);
    }

protected:
    funcCloud           m_cloudFunction;
    funcCloudIndex      m_cloudIndexFunction;
    int                 m_index;
};

#endif // CT_STANDARDGLOBALCLOUDITEMDATAVALUEDELEGATET_H
