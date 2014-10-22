#include "dm_attributesnormalt.h"

#include "ct_global/ct_context.h"

template<>
bool DM_AttributesNormalT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();
    const CT_AbstractPointCloud *cloud = PS_REPOSITORY->globalPointCloud();

    size_t size = index->size();
    size_t indexP;

    QSharedPointer<CT_StandardNormalCloudRegistered> spn = doc->normalCloudRegistered<CT_AbstractPointsAttributes>();

    if(spn.data() != NULL)
    {
        CT_AbstractNormalCloud *nn = spn->abstractNormalCloud();

        for(size_t i=0; i<size && !isCanceled(); ++i)
        {
            index->indexAt(i, indexP);
            const CT_Normal &nxnynz_pa = m_an->constNormalAt(i);
            const CT_Point &xyz = cloud->constTAt(indexP);

            // set the normal of the point at this document
            CT_Normal &nxnynz = nn->normalAt(indexP);
            nxnynz.normal_x = xyz.getX() + nxnynz_pa.normal_x;
            nxnynz.normal_y = xyz.getY() + nxnynz_pa.normal_y;
            nxnynz.normal_z = xyz.getZ() + nxnynz_pa.normal_z;

            setProgress((i*100)/size);
        }

        doc->redrawGraphics();

        return true;
    }

    return false;
}

template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

    size_t size = index->size();
    size_t indexF;

    QSharedPointer<CT_StandardNormalCloudRegistered> spn = doc->normalCloudRegistered<CT_AbstractFaceAttributes>();

    if(spn.data() != NULL)
    {
        CT_AbstractNormalCloud *nn = spn->abstractNormalCloud();

        for(size_t i=0; i<size && !isCanceled(); ++i)
        {
            index->indexAt(i, indexF);
            const CT_Normal &nxnynz_pa = m_an->constNormalAt(i);

            // set the normal of the face at this document
            CT_Normal &nxnynz = nn->normalAt(indexF);
            nxnynz.setNormal(nxnynz_pa);

            setProgress((i*100)/size);
        }

        doc->redrawGraphics();

        return true;
    }

    return false;
}

template class DM_AttributesNormalT<CT_AbstractPointsAttributes>;
template class DM_AttributesNormalT<CT_AbstractFaceAttributes>;
