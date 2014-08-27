#ifndef DM_ATTRIBUTESNORMALT_HPP
#define DM_ATTRIBUTESNORMALT_HPP

#include "tools/attributes/worker/dm_attributesnormalt.h"

#include "ct_global/ct_context.h"

template<typename Type, typename TypeCloudIndex>
DM_AttributesNormalT<Type, TypeCloudIndex>::DM_AttributesNormalT() : DM_AbstractAttributesNormal()
{
    m_an = NULL;
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesNormalT<Type, TypeCloudIndex>::checkAndSetNormalCloudToDoc()
{
    GDocumentViewForGraphics *doc = document();

    if(doc->normalCloudRegistered<Type>().data() == NULL)
        doc->setNormalCloudRegistered<Type>(PS_REPOSITORY->createNewNormalCloud(this->syncWithCloud()));
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesNormalT<Type, TypeCloudIndex>::setTypeAttributes(const Type *ta, const CT_AttributesNormal *an)
{
    if(ta != dynamic_cast<const Type*>(an))
        return false;

    setAttributes(ta);
    m_an = (CT_AttributesNormal*)an;

    return true;
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesNormalT<Type, TypeCloudIndex>::attributesDeleted()
{
    m_an = NULL;
}

template<typename Type, typename TypeCloudIndex>
CT_AttributesNormal* DM_AttributesNormalT<Type, TypeCloudIndex>::normalAttributes() const
{
    return m_an;
}

template<typename Type, typename TypeCloudIndex>
Type* DM_AttributesNormalT<Type, TypeCloudIndex>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESNORMALT_HPP
