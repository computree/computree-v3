#include "ct_laspointformat10.h"

CT_LASPointFormat10::CT_LASPointFormat10(): CT_LASPointFormat9()
{
}

size_t CT_LASPointFormat10::sizeInBytes() const
{
    return CT_LASPointFormat9::sizeInBytes() + colorPacketSizeInBytes() + nirPacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat10::write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex)
{
    CT_LasPointInfo *info = CT_LASPointFormat6::write(stream, p, globalIndex);

    writeInfoFormat10(stream, info);
    writeInfoFormat9(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat10::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat9::typesToSearch();
    l.append(colorPacketTypesNeeded());
    l.append(nirPacketTypesNeeded());

    return l;
}

void CT_LASPointFormat10::writeInfoFormat10(QDataStream &stream, CT_LasPointInfo *info)
{
    writeColorPacket(stream, info);
    writeNirPacket(stream, info);
}
