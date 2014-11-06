#include "ct_laspointformat5.h"

CT_LASPointFormat5::CT_LASPointFormat5() : CT_LASPointFormat3()
{
}

size_t CT_LASPointFormat5::sizeInBytes() const
{
    return CT_LASPointFormat3::sizeInBytes() + wavePacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat5::write(QDataStream &stream, const size_t &globalIndex)
{
    CT_LasPointInfo *info = CT_LASPointFormat3::write(stream, globalIndex);

    writeInfoFormat5(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat5::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat3::typesToSearch();
    l.append(wavePacketTypesNeeded());

    return l;
}

void CT_LASPointFormat5::writeInfoFormat5(QDataStream &stream, CT_LasPointInfo *info)
{
    writeWavePacket(stream, info);
}
