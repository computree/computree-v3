#include "ct_lasnirpacket.h"

#include "abstract/ct_abstractlaspointformat.h"

CT_LASNirPacket::CT_LASNirPacket()
{
}

size_t CT_LASNirPacket::nirPacketSizeInBytes() const
{
    return sizeof(m_emptyData);
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASNirPacket::nirPacketTypesNeeded()
{
    QList<CT_LasDefine::LASPointAttributesType> l;

    l.append(CT_LasDefine::NIR);

    return l;
}

void CT_LASNirPacket::writeNirPacket(QDataStream &stream, CT_LasPointInfo *info)
{
    if(info != NULL)
    {
        CT_WRITE_LAS_SCALAR(info->m_nir, quint16) // nir
    }
    else
    {
        stream.writeRawData(&m_emptyData[0], sizeof(m_emptyData));
    }
}
