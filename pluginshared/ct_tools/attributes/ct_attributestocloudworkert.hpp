#ifndef CT_ATTRIBUTESTOCLOUDWORKERT_HPP
#define CT_ATTRIBUTESTOCLOUDWORKERT_HPP

#include "ct_tools/attributes/ct_attributestocloudworkert.h"

#include "ct_attributes/abstract/ct_abstractattributesscalar.h"
#include "ct_attributes/ct_attributescolor.h"
#include "ct_attributes/ct_attributesnormal.h"

template<typename TypeAttribute>
void CT_AttributesToCloudWorkerT<TypeAttribute>::apply()
{
    m_cancel = false;
    setProgress(0);

    if(!m_attributes.isEmpty())
    {
        CT_AbstractColorCloud *cc = NULL;
        CT_AbstractNormalCloud *nn = NULL;

        if(m_colors.data() != NULL)
            cc = m_colors->abstractColorCloud();

        if(m_normals.data() != NULL)
            nn = m_normals->abstractNormalCloud();

        size_t totalSize = 0;

        QListIterator<TypeAttribute*> it(m_attributes);

        while(it.hasNext())
            totalSize += it.next()->attributesSize();

        size_t currentSize = 0;

        it.toFront();
        while(it.hasNext()
              && !m_cancel)
        {
            TypeAttribute *pa = it.next();
            const CT_AbstractCloudIndex *pci = pa->abstractCloudIndex();
            size_t s = pa->attributesSize();
            size_t indexP;

            if(s > 0)
            {
                bool used = false;

                if(cc != NULL)
                {
                    CT_AbstractAttributesScalar *pas = dynamic_cast<CT_AbstractAttributesScalar*>(pa);

                    if(pas != NULL)
                    {
                        used = true;

                        double min = pas->dMin();
                        double max = pas->dMax();
                        double range = max-min;

                        if(range == 0)
                            range = 1;

                        for(size_t i=0; i<s && !m_cancel; ++i)
                        {
                            pci->indexAt(i, indexP);
                            CT_Color &color = cc->colorAt(indexP);

                            color.r = ((pas->dValueAt(i)-min)*255)/range;
                            color.g = color.r;
                            color.b = color.r;

                            ++currentSize;

                            setProgress((currentSize*100)/totalSize);
                        }
                    }
                    else
                    {
                        CT_AttributesColor *pac = dynamic_cast<CT_AttributesColor*>(pa);

                        if(pac != NULL)
                        {
                            used = true;

                            for(size_t i=0; i<s && !m_cancel; ++i)
                            {
                                pci->indexAt(i, indexP);
                                CT_Color &color1 = cc->colorAt(indexP);
                                const CT_Color &color2 = pac->constColorAt(i);

                                color1.setColor(color2);
                                ++currentSize;

                                setProgress((currentSize*100)/totalSize);
                            }
                        }
                    }
                }

                if(nn != NULL)
                {
                    CT_AttributesNormal *pan = dynamic_cast<CT_AttributesNormal*>(pa);

                    if(pan != NULL)
                    {
                        used = true;

                        for(size_t i=0; i<s && !m_cancel; ++i)
                        {
                            pci->indexAt(i, indexP);
                            CT_Normal &normal1 = nn->normalAt(indexP);
                            const CT_Normal &normal2 = pan->constNormalAt(i);

                            normal1.setNormal(normal2);

                            ++currentSize;

                            setProgress((currentSize*100)/totalSize);
                        }
                    }
                }

                if(!used)
                {
                    currentSize += s;
                    setProgress((currentSize*100)/totalSize);
                }
            }
        }
    }

    setProgress(100);
    emit finished();
}

#endif // CT_ATTRIBUTESTOCLOUDWORKERT_HPP
