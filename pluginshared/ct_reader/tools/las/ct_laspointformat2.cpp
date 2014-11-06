#include "ct_laspointformat2.h"

CT_LASPointFormat2::CT_LASPointFormat2() : CT_LASPointFormat0()
{
}

size_t CT_LASPointFormat2::sizeInBytes() const
{
    return CT_LASPointFormat0::sizeInBytes() + colorPacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat2::write(QDataStream &stream, const size_t &globalIndex)
{
    CT_LasPointInfo *info = CT_LASPointFormat0::write(stream, globalIndex);

    writeInfoFormat2(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat2::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat0::typesToSearch();
    l.append(colorPacketTypesNeeded());

    return l;
}

void CT_LASPointFormat2::writeInfoFormat2(QDataStream &stream, CT_LasPointInfo *info)
{
    writeColorPacket(stream, info);
}
