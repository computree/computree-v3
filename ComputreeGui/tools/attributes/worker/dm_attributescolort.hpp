#ifndef DM_ATTRIBUTESCOLORT_HPP
#define DM_ATTRIBUTESCOLORT_HPP

#include "tools/attributes/worker/dm_attributescolort.h"

template<typename Type>
DM_AttributesColorT<Type>::DM_AttributesColorT() : DM_AbstractAttributesColor()
{
    m_ac = NULL;
}

template<typename Type>
bool DM_AttributesColorT<Type>::setTypeAttributes(const Type *ta, const CT_AttributesColor *ac)
{
    if(ta != dynamic_cast<const Type*>(ac))
        return false;

    setAttributes(ta);
    m_ac = (CT_AttributesColor*)ac;

    return true;
}

template<typename Type>
void DM_AttributesColorT<Type>::attributesDeleted()
{
    m_ac = NULL;
}

template<typename Type>
CT_AttributesColor* DM_AttributesColorT<Type>::colorAttributes() const
{
    return m_ac;
}

template<typename Type>
Type* DM_AttributesColorT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESCOLORT_HPP
