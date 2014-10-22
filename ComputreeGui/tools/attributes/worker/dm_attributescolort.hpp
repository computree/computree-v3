#ifndef DM_ATTRIBUTESCOLORT_HPP
#define DM_ATTRIBUTESCOLORT_HPP

#include "tools/attributes/worker/dm_attributescolort.h"

template<typename Type>
DM_AttributesColorT<Type>::DM_AttributesColorT() : DM_AbstractAttributesColor()
{
    m_ac = NULL;
}

template<typename Type>
void DM_AttributesColorT<Type>::checkAndSetNecessaryCloudToDoc()
{
    GDocumentViewForGraphics *doc = document();

    if(doc->colorCloudRegistered<Type>().data() == NULL)
        doc->createColorCloudRegistered<Type>();
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
bool DM_AttributesColorT<Type>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        QSharedPointer<CT_StandardColorCloudRegistered> docSharedColorCloud = doc->colorCloudRegistered<Type>();

        if(docSharedColorCloud.data() != NULL)
        {
            CT_AbstractColorCloud *docColorCloud = docSharedColorCloud->abstractColorCloud();
            CT_AbstractColorCloud *toApplyColorCloud = m_ac->getColorCloud();

            size_t size = index->size();
            size_t indexP;

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, indexP);
                const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                // set the color of the Type at this document
                CT_Color &color = docColorCloud->colorAt(indexP);
                color.setColor(color_pa);

                setProgress((i*100)/size);
            }

            doc->redrawGraphics();

            return true;
        }
    }

    return false;
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
