#include "dm_attributesscalart.h"

template<>
void DM_AttributesScalarT<CT_AbstractPointsAttributes>::staticApply(ConcurrentMapInfo *info)
{
    size_t globalPointIndex;

    for(size_t i=info->m_begin; i<info->m_end; ++i)
    {
        double vv = info->m_as->dValueAt(i);
        // convert the value to be between 0 and 1
        double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        QColor color = info->m_interpolator->intermediateColor(key);

        info->m_index->indexAt(i, globalPointIndex);
        // set the color of the point at this document
        CT_Color &colorC = info->m_cc->at(globalPointIndex);
        colorC.b() = color.blue();
        colorC.g() = color.green();
        colorC.r() = color.red();
        colorC.a() = color.alpha();
    }
}

template<>
void DM_AttributesScalarT<CT_AbstractFaceAttributes>::staticApply(ConcurrentMapInfo *info)
{
    size_t globalFaceIndex;

    for(size_t i=info->m_begin; i<info->m_end; ++i)
    {
        double vv = info->m_as->dValueAt(i);
        // convert the value to be between 0 and 1
        double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        QColor color = info->m_interpolator->intermediateColor(key);

        info->m_index->indexAt(i, globalFaceIndex);

        const CT_Face &face = info->m_fAccess->constFaceAt(globalFaceIndex);

        // set the color of the face at this document
        CT_Color &colorA = info->m_cc->at(face.iPointAt(0));
        colorA.b() = color.blue();
        colorA.g() = color.green();
        colorA.r() = color.red();
        colorA.a() = color.alpha();

        info->m_cc->at(face.iPointAt(1)).setColor(colorA);
        info->m_cc->at(face.iPointAt(2)).setColor(colorA);
    }
}

template<>
void DM_AttributesScalarT<CT_AbstractEdgeAttributes>::staticApply(ConcurrentMapInfo *info)
{
    size_t globalEdgeIndex;

    for(size_t i=info->m_begin; i<info->m_end; ++i)
    {
        double vv = info->m_as->dValueAt(i);
        // convert the value to be between 0 and 1
        double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        QColor color = info->m_interpolator->intermediateColor(key);

        info->m_index->indexAt(i, globalEdgeIndex);

        const CT_Edge &edge = info->m_eAccess->constEdgeAt(globalEdgeIndex);

        // set the color of the face at this document
        CT_Color &colorA = info->m_cc->at(edge.iPointAt(0));
        colorA.b() = color.blue();
        colorA.g() = color.green();
        colorA.r() = color.red();
        colorA.a() = color.alpha();

        info->m_cc->at(edge.iPointAt(1)).setColor(colorA);
    }
}

template class DM_AttributesScalarT<CT_AbstractPointsAttributes>;
template class DM_AttributesScalarT<CT_AbstractFaceAttributes>;
template class DM_AttributesScalarT<CT_AbstractEdgeAttributes>;
