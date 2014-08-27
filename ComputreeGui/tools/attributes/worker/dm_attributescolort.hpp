#ifndef DM_ATTRIBUTESCOLORT_HPP
#define DM_ATTRIBUTESCOLORT_HPP

#include "tools/attributes/worker/dm_attributescolort.h"

#include "dm_guimanager.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"

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
bool DM_AttributesColorT<Type, TypeCloudIndex>::setTypeAttributes(const Type *ta, const CT_AttributesColor *ac)
{
    if(ta != dynamic_cast<const Type*>(ac))
        return false;

    setAttributes(ta);
    m_ac = (CT_AttributesColor*)ac;

    return true;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesColorT<Type, TypeCloudIndex>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const TypeCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();
        size_t size = index->size();
        size_t indexP;

        QSharedPointer<CT_StandardColorCloudRegistered> spcc = doc->colorCloudRegistered<Type>();

        if(spcc.data() != NULL)
        {
            CT_AbstractColorCloud *cc = spcc->abstractColorCloud();

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, indexP);
                const CT_Color &color_pa = m_ac->constColorAt(i);

                // set the color of the Type at this document
                CT_Color &color = cc->colorAt(indexP);
                color.setColor(color_pa);

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
CT_AttributesColor* DM_AttributesColorT<Type, TypeCloudIndex>::colorAttributes() const
{
    return m_ac;
}

template<typename Type, typename TypeCloudIndex>
Type* DM_AttributesColorT<Type, TypeCloudIndex>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

#endif // DM_ATTRIBUTESCOLORT_HPP
