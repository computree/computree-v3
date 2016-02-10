#include "ct_laswavepacket.h"

#include "abstract/ct_abstractlaspointformat.h"

CT_LASWavePacket::CT_LASWavePacket()
{
}

size_t CT_LASWavePacket::wavePacketSizeInBytes() const
{
    return sizeof(m_emptyData);
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASWavePacket::wavePacketTypesNeeded()
{
    QList<CT_LasDefine::LASPointAttributesType> l;

    l.append(CT_LasDefine::Wave_Packet_Descriptor_Index);
    l.append(CT_LasDefine::Byte_offset_to_waveform_data);
    l.append(CT_LasDefine::Waveform_packet_size_in_bytes);
    l.append(CT_LasDefine::Return_Point_Waveform_Location);

    return l;
}

void CT_LASWavePacket::writeWavePacket(QDataStream &stream, CT_LasPointInfo *info)
{
    if(info != NULL)
    {
        // wave packet descriptor index
        CT_WRITE_LAS_SCALAR(info->m_wpdi, quint8);

        // byte offset to waveform data
        CT_WRITE_LAS_SCALAR(info->m_bowd, quint64);

        // waveform packet size in bytes
        CT_WRITE_LAS_SCALAR(info->m_wpsb, quint32);

        // return point waveform location
        CT_WRITE_LAS_SCALAR(info->m_rpwl, float);

        // xt
        stream << (float)0;

        // yt
        stream << (float)0;

        // zt
        stream << (float)0;
    }
    else
        stream.writeRawData(&m_emptyData[0], sizeof(m_emptyData));
}
