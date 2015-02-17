#include "ct_laspointformat3.h"

CT_LASPointFormat3::CT_LASPointFormat3() : CT_LASPointFormat2()
{
}

size_t CT_LASPointFormat3::sizeInBytes() const
{
    return CT_LASPointFormat2::sizeInBytes() + gpsPacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat3::write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex)
{
    CT_LasPointInfo *info = CT_LASPointFormat0::write(stream, p, globalIndex);

    writeInfoFormat3(stream, info);
    CT_LASPointFormat2::writeInfoFormat2(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat3::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat2::typesToSearch();
    l.append(gpsPacketTypesNeeded());

    return l;
}

void CT_LASPointFormat3::writeInfoFormat3(QDataStream &stream, CT_LasPointInfo *info)
{
    writeGpsPacket(stream, info);
}
