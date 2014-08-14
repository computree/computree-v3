#ifndef DM_ATTRIBUTESNORMALT_HPP
#define DM_ATTRIBUTESNORMALT_HPP

#include "tools/attributes/worker/dm_attributesnormalt.h"

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
        doc->setNormalCloudRegistered<Type>(GUI_MANAGER->getPluginsContext()->repository()->createNewNormalCloud(this->syncWithCloud()));
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesNormalT<Type, TypeCloudIndex>::setTypeAttributes(const Type *ta, const IAttributesNormal *an)
{
    if(ta != dynamic_cast<const Type*>(an))
        return false;

    setAttributes(ta);
    m_an = (IAttributesNormal*)an;

    return true;
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesNormalT<Type, TypeCloudIndex>::attributesDeleted()
{
    m_an = NULL;
}

template<typename Type, typename TypeCloudIndex>
IAttributesNormal* DM_AttributesNormalT<Type, TypeCloudIndex>::normalAttributes() const
{
    return m_an;
}

template<typename Type, typename TypeCloudIndex>
Type* DM_AttributesNormalT<Type, TypeCloudIndex>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESNORMALT_HPP
