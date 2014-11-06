#include "ct_laspointformat9.h"

CT_LASPointFormat9::CT_LASPointFormat9() : CT_LASPointFormat6()
{
}

size_t CT_LASPointFormat9::sizeInBytes() const
{
    return CT_LASPointFormat6::sizeInBytes() + wavePacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat9::write(QDataStream &stream, const size_t &globalIndex)
{
    CT_LasPointInfo *info = CT_LASPointFormat6::write(stream, globalIndex);

    writeInfoFormat9(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat9::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat6::typesToSearch();
    l.append(wavePacketTypesNeeded());

    return l;
}

void CT_LASPointFormat9::writeInfoFormat9(QDataStream &stream, CT_LasPointInfo *info)
{
    writeWavePacket(stream, info);
}
