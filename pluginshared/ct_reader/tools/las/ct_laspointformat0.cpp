#include "ct_laspointformat0.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_global/ct_context.h"

CT_LASPointFormat0::CT_LASPointFormat0() : CT_AbstractLASPointFormat()
{
    m_pCloud = PS_REPOSITORY->globalPointCloud();
}

size_t CT_LASPointFormat0::sizeInBytes() const
{
    return sizeof(m_emptyData);
}

CT_LasPointInfo* CT_LASPointFormat0::write(QDataStream &stream, const size_t &globalIndex)
{
    CT_LasPointInfo *info = infoOfPoint(globalIndex);

    const CT_Point &point = m_pCloud->constTAt(globalIndex);

    PS_COORDINATES_SYS->convertExport(point(0), point(1), point(2), m_xc, m_yc, m_zc);

    header()->inverseTransformPoint(m_xc, m_yc, m_zc, m_x, m_y, m_z);

    // x / y / z
    stream << m_x << m_y << m_z;

    if(info != NULL)
    {
        // intensity
        CT_WRITE_LAS_SCALAR(info->m_i, quint16);

        // return number + number of return + scan direction flag + edge of flight line
        quint8 tmp = 0;

        if(info->m_rn.first != NULL)
            tmp |= (((quint8)info->m_rn.first->dValueAt(info->m_rn.second)) << 0);

        if(info->m_nor.first != NULL)
            tmp |= (((quint8)info->m_nor.first->dValueAt(info->m_nor.second)) << 3);

        if(info->m_sdf.first != NULL)
            tmp |= (((quint8)info->m_sdf.first->dValueAt(info->m_sdf.second)) << 6);

        if(info->m_efl.first != NULL)
            tmp |= (((quint8)info->m_efl.first->dValueAt(info->m_efl.second)) << 7);

        stream << tmp;

        // classification
        CT_WRITE_LAS_SCALAR(info->m_c, quint8);

        // scan angle rank
        CT_WRITE_LAS_SCALAR(info->m_sar, qint8);

        // user data
        CT_WRITE_LAS_SCALAR(info->m_ud, quint8);

        // point source ID
        CT_WRITE_LAS_SCALAR(info->m_psID, quint16);
    }
    else
    {
        stream.writeRawData(&m_emptyData[0], sizeof(m_emptyData) - (sizeof(m_x)*3));
    }

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat0::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l;
    l << CT_LasDefine::Intensity;
    l << CT_LasDefine::Return_Number;
    l << CT_LasDefine::Number_of_Returns;
    l << CT_LasDefine::Scan_Direction_Flag;
    l << CT_LasDefine::Edge_of_Flight_Line;
    l << CT_LasDefine::Classification;
    l << CT_LasDefine::Scan_Angle_Rank;
    l << CT_LasDefine::User_Data;
    l << CT_LasDefine::Point_Source_ID;

    return l;
}
