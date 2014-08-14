#ifndef CT_STANDARDITEMDATAVALUEDELEGATESTATICT_H
#define CT_STANDARDITEMDATAVALUEDELEGATESTATICT_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdatavaluedelegate.h"

template<typename DataType>
class CT_StandardItemDataValueDelegateStaticT : public CT_AbstractItemDataValueDelegate
{
public:
    CT_StandardItemDataValueDelegateStaticT(const DataType yourValue) : CT_AbstractItemDataValueDelegate()
    {
        m_value = yourValue;
    }

    bool value(const CT_AbstractItemDrawable *item, IItemDataValue* valueToSet) const
    {
        Q_UNUSED(item)

        ((CT_StandardItemDataValue*)valueToSet)->setValue(m_value);
        return true;
    }

    IItemDataValue::ItemDataValueType dataType() const
    {
        CT_StandardItemDataValue d;
        d.setValue(m_value);

        return d.type();
    }

    CT_AbstractItemDataValueDelegate* copy() const
    {
        return new CT_StandardItemDataValueDelegateStaticT<DataType>(m_value);
    }

private:
    DataType    m_value;
};

#endif // CT_STANDARDITEMDATAVALUEDELEGATESTATICT_H
