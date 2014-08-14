#ifndef CT_STANDARDITEMDATAVALUEDELEGATET_H
#define CT_STANDARDITEMDATAVALUEDELEGATET_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdatavaluedelegate.h"

template<class ItemDrawableClass, typename DataType>
class CT_StandardItemDataValueDelegateT : public CT_AbstractItemDataValueDelegate
{
public:
    typedef DataType (ItemDrawableClass::*func)() const;

    CT_StandardItemDataValueDelegateT(func yourFunction) : CT_AbstractItemDataValueDelegate()
    {
        m_itemFunction = yourFunction;
    }

    IItemDataValue::ItemDataValueType dataType() const
    {
        return (IItemDataValue::ItemDataValueType)(CT_StandardItemDataValue::staticDataTypeToScalarType<DataType>() + ((int)IItemDataValue::IDVT_BOOL));
    }

    bool value(const CT_AbstractItemDrawable *item, IItemDataValue* valueToSet) const
    {
        ((CT_StandardItemDataValue*)valueToSet)->setValue((dynamic_cast<const ItemDrawableClass*>(item)->*m_itemFunction)());
        return true;
    }

    CT_AbstractItemDataValueDelegate* copy() const
    {
        return new CT_StandardItemDataValueDelegateT<ItemDrawableClass, DataType>(this->m_itemFunction);
    }

protected:
    func            m_itemFunction;
};

#endif // CT_STANDARDITEMDATAVALUEDELEGATET_H
