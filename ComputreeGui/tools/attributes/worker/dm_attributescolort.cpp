#include "dm_attributescolort.h"

template<>
bool DM_AttributesColorT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        osg::ref_ptr<GOsgGraphicsView::ColorArrayType> colorArray = doc->getOrCreateGlobalColorArrayForPoints();

        if(colorArray.valid())
        {
            CT_AbstractColorCloud *toApplyColorCloud = m_ac->getColorCloud();

            size_t size = index->size();
            size_t globalIndexOfPoint;

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, globalIndexOfPoint);
                const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                // set the color of the Type at this document
                CT_Color &color = colorArray->at(globalIndexOfPoint);
                color.setColor(color_pa);

                setProgress((i*100)/size);
            }

            doc->dirtyColorsOfPoints();

            return true;
        }
    }

    return false;
}

template<>
bool DM_AttributesColorT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        osg::ref_ptr<GOsgGraphicsView::ColorArrayType> colorArray = doc->getOrCreateGlobalColorArrayForPoints();

        if(colorArray.valid())
        {
            CT_FaceAccessor fAccess;

            CT_AbstractColorCloud *toApplyColorCloud = m_ac->getColorCloud();

            size_t size = index->size();
            size_t globalIndexOfFace;

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, globalIndexOfFace);
                const CT_Face &face = fAccess.constFaceAt(globalIndexOfFace);

                const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                // set the color of the points of the face
                colorArray->at(face.iPointAt(0)).setColor(color_pa);
                colorArray->at(face.iPointAt(1)).setColor(color_pa);
                colorArray->at(face.iPointAt(2)).setColor(color_pa);

                setProgress((i*100)/size);
            }

            doc->dirtyColorsOfPoints();

            return true;
        }
    }

    return false;
}

template<>
bool DM_AttributesColorT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != NULL)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        osg::ref_ptr<GOsgGraphicsView::ColorArrayType> colorArray = doc->getOrCreateGlobalColorArrayForPoints();

        if(colorArray.valid())
        {
            CT_EdgeAccessor eAccess;

            CT_AbstractColorCloud *toApplyColorCloud = m_ac->getColorCloud();

            size_t size = index->size();
            size_t globalIndexOfEdge;

            for(size_t i=0; i<size && !isCanceled(); ++i)
            {
                index->indexAt(i, globalIndexOfEdge);
                const CT_Edge &edge = eAccess.constEdgeAt(globalIndexOfEdge);

                const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                // set the color of the points of the edge
                colorArray->at(edge.iPointAt(0)).setColor(color_pa);
                colorArray->at(edge.iPointAt(1)).setColor(color_pa);

                setProgress((i*100)/size);
            }

            doc->dirtyColorsOfPoints();

            return true;
        }
    }

    return false;
}

template class DM_AttributesColorT<CT_AbstractPointsAttributes>;
template class DM_AttributesColorT<CT_AbstractFaceAttributes>;
template class DM_AttributesColorT<CT_AbstractEdgeAttributes>;
