#include "dm_attributesnormalt.h"

template<>
bool DM_AttributesNormalT<IPointAttributes, ICloudIndex>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    const ICloudIndex *index = abstractTypeAttributes()->cloudIndex();
    const IPointCloud *cloud = GUI_MANAGER->getPluginsContext()->repository()->globalPointsCloud();

    size_t size = index->indexSize();
    size_t indexP;

    QSharedPointer<NormalCloudRegisteredInterface> spn = doc->normalCloudRegistered<IPointAttributes>();

    if(spn.data() != NULL)
    {
        INormalCloud *nn = spn->normalCloud();

        for(size_t i=0; i<size && !isCanceled(); ++i)
        {
            index->indexAt(i, indexP);
            float *nxnynz_pa = m_an->valueAt(i);
            float *xyz = cloud->valueAt(indexP);

            // set the normal of the point at this document
            float *nxnynz = nn->valueAt(indexP);
            *nxnynz = (*xyz) + (*nxnynz_pa);
            *(nxnynz+1) = (*(xyz+1)) + (*(nxnynz_pa+1));
            *(nxnynz+2) = (*(xyz+2)) + (*(nxnynz_pa+2));

            setProgress((i*100)/size);
        }

        doc->redrawGraphics();

        return true;
    }

    return false;
}

template<>
bool DM_AttributesNormalT<IFaceAttributes, ICloudIndex>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    const ICloudIndex *index = abstractTypeAttributes()->cloudIndex();

    size_t size = index->indexSize();
    size_t indexF;

    QSharedPointer<NormalCloudRegisteredInterface> spn = doc->normalCloudRegistered<IFaceAttributes>();

    if(spn.data() != NULL)
    {
        INormalCloud *nn = spn->normalCloud();

        for(size_t i=0; i<size && !isCanceled(); ++i)
        {
            index->indexAt(i, indexF);
            float *nxnynz_pa = m_an->valueAt(i);

            // set the normal of the face at this document
            float *nxnynz = nn->valueAt(indexF);
            *nxnynz = *nxnynz_pa;
            *(nxnynz+1) = *(nxnynz_pa+1);
            *(nxnynz+2) = *(nxnynz_pa+2);

            setProgress((i*100)/size);
        }

        doc->redrawGraphics();

        return true;
    }

    return false;
}

template class DM_AttributesNormalT<IPointAttributes, ICloudIndex>;
template class DM_AttributesNormalT<IFaceAttributes, ICloudIndex>;
