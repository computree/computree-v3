#ifndef DM_ATTRIBUTESNORMALT_HPP
#define DM_ATTRIBUTESNORMALT_HPP

#include "tools/attributes/worker/dm_attributesnormalt.h"

#include "ct_global/ct_context.h"

template<typename Type>
DM_AttributesNormalT<Type>::DM_AttributesNormalT() : DM_AbstractAttributesNormal()
{
    m_an = NULL;
}

template<typename Type>
void DM_AttributesNormalT<Type>::checkAndSetNecessaryCloudToDoc()
{
    GDocumentViewForGraphics *doc = document();

    if(doc->normalCloudRegistered<Type>().data() == NULL)
        doc->setNormalCloudRegistered<Type>(PS_REPOSITORY->createNewNormalCloud(this->syncWithCloud()));
}

template<typename Type>
bool DM_AttributesNormalT<Type>::setTypeAttributes(const Type *ta, const CT_AttributesNormal *an)
{
    if(ta != dynamic_cast<const Type*>(an))
        return false;

    setAttributes(ta);
    m_an = (CT_AttributesNormal*)an;

    return true;
}

template<typename Type>
void DM_AttributesNormalT<Type>::attributesDeleted()
{
    m_an = NULL;
}

template<typename Type>
CT_AttributesNormal* DM_AttributesNormalT<Type>::normalAttributes() const
{
    return m_an;
}

template<typename Type>
Type* DM_AttributesNormalT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESNORMALT_HPP
