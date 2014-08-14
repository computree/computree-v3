#ifndef CT_STANDARDARRAYTITEMDATAVALUEDELEGATET_H
#define CT_STANDARDARRAYTITEMDATAVALUEDELEGATET_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdatavaluedelegate.h"

template<class ItemDrawableClass, template< typename DataType > class ArrayType, typename DataType>
class CT_StandardArrayTItemDataValueDelegateT : public CT_AbstractItemDataValueDelegate
{
public:
    typedef const ArrayType<DataType>* (ItemDrawableClass::*func)() const;

    CT_StandardArrayTItemDataValueDelegateT(func yourFunction) : CT_AbstractItemDataValueDelegate()
    {
        m_itemFunction = yourFunction;
    }

    IItemDataValue::ItemDataValueType dataType() const
    {
        return (IItemDataValue::ItemDataValueType)CT_StandardItemDataValue::staticDataTypeToArrayTType< ArrayType<DataType>, DataType>();
    }

    bool value(const CT_AbstractItemDrawable *item, IItemDataValue* valueToSet) const
    {
        ((CT_StandardItemDataValue*)valueToSet)->setValue((dynamic_cast<const ItemDrawableClass*>(item)->*m_itemFunction)());
        return true;
    }

    CT_AbstractItemDataValueDelegate* copy() const
    {
        return new CT_StandardArrayTItemDataValueDelegateT<ItemDrawableClass, ArrayType, DataType>(this->m_itemFunction);
    }

protected:
    func            m_itemFunction;
};

#endif // CT_STANDARDARRAYTITEMDATAVALUEDELEGATET_H
