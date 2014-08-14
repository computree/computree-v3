#ifndef DM_ATTRIBUTESCOLORT_HPP
#define DM_ATTRIBUTESCOLORT_HPP

#include "tools/attributes/worker/dm_attributescolort.h"

#include "dm_guimanager.h"

template<typename Type, typename TypeCloudIndex>
DM_AttributesColorT<Type, TypeCloudIndex>::DM_AttributesColorT() : DM_AbstractAttributesColor()
{
    m_ac = NULL;
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesColorT<Type, TypeCloudIndex>::checkAndSetColorCloudToDoc()
{
    GDocumentViewForGraphics *doc = document();

    if(doc->colorCloudRegistered<Type>().data() == NULL)
        doc->createColorCloudRegistered<Type>();
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesColorT<Type, TypeCloudIndex>::setTypeAttributes(const Type *ta, const IAttributesColor *ac)
{
    if(ta != dynamic_cast<const Type*>(ac))
        return false;

    setAttributes(ta);
    m_ac = (IAttributesColor*)ac;

    return true;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesColorT<Type, TypeCloudIndex>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const TypeCloudIndex *index = abstractTypeAttributes()->cloudIndex();
        size_t size = index->indexSize();
        size_t indexP;

        QSharedPointer<ColorCloudRegisteredInterface> spcc = doc->colorCloudRegistered<Type>();

        if(spcc.data() != NULL)
        {
            IColorCloud *cc = spcc->colorCloud();

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, indexP);
                quint8 *bgra_pa = m_ac->valueAt(i);

                // set the color of the Type at this document
                quint8 *bgra = cc->valueAt(indexP);
                *bgra = *bgra_pa;
                *(bgra+1) = *(bgra_pa+1);
                *(bgra+2) = *(bgra_pa+2);

                setProgress((i*100)/size);
            }

            doc->redrawGraphics();

            return true;
        }
    }

    return false;
}


template<typename Type, typename TypeCloudIndex>
void DM_AttributesColorT<Type, TypeCloudIndex>::attributesDeleted()
{
    m_ac = NULL;
}

template<typename Type, typename TypeCloudIndex>
IAttributesColor* DM_AttributesColorT<Type, TypeCloudIndex>::colorAttributes() const
{
    return m_ac;
}

template<typename Type, typename TypeCloudIndex>
Type* DM_AttributesColorT<Type, TypeCloudIndex>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESCOLORT_HPP
