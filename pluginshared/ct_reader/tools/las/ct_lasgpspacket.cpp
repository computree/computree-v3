#include "ct_lasgpspacket.h"

#include "ct_reader/tools/las/abstract/ct_abstractlaspointformat.h"

CT_LASGpsPacket::CT_LASGpsPacket()
{
}

size_t CT_LASGpsPacket::gpsPacketSizeInBytes() const
{
    return sizeof(m_emptyData);
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASGpsPacket::gpsPacketTypesNeeded()
{
    QList<CT_LasDefine::LASPointAttributesType> l;

    l.append(CT_LasDefine::GPS_Time);

    return l;
}

void CT_LASGpsPacket::writeGpsPacket(QDataStream &stream, CT_LasPointInfo *info)
{
    if(info != NULL)
    {
        CT_WRITE_LAS_SCALAR(info->m_gpsT, double) // gps time
    }
    else
    {
        stream.writeRawData(&m_emptyData[0], sizeof(m_emptyData));
    }
}

