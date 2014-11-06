#include "ct_lascolorpacket.h"

#include "ct_reader/tools/las/abstract/ct_abstractlaspointformat.h"

CT_LASColorPacket::CT_LASColorPacket()
{
}

size_t CT_LASColorPacket::colorPacketSizeInBytes() const
{
    return sizeof(m_emptyData);
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASColorPacket::colorPacketTypesNeeded()
{
    QList<CT_LasDefine::LASPointAttributesType> l;

    l.append(CT_LasDefine::Red);
    l.append(CT_LasDefine::Green);
    l.append(CT_LasDefine::Blue);

    return l;
}

void CT_LASColorPacket::writeColorPacket(QDataStream &stream, CT_LasPointInfo *info)
{
    if(info != NULL)
    {
        // red
        CT_WRITE_LAS_SCALAR(info->m_red, quint16);

        // green
        CT_WRITE_LAS_SCALAR(info->m_green, quint16);

        // blue
        CT_WRITE_LAS_SCALAR(info->m_blue, quint16);
    }
    else
        stream.writeRawData(&m_emptyData[0], sizeof(m_emptyData));
}
