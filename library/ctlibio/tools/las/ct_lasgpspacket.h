#ifndef CT_LASGPSPACKET_H
#define CT_LASGPSPACKET_H

#include "ct_lasdefine.h"
#include "ct_laspointinfo.h"

#include <QDataStream>

class CTLIBIO_EXPORT CT_LASGpsPacket
{
public:
    CT_LASGpsPacket();

    /**
     * @brief Return the size of the gps packet in bytes
     */
    size_t gpsPacketSizeInBytes() const;

    /**
     * @brief Returns the types needed to write the gps packet
     */
    QList<CT_LasDefine::LASPointAttributesType> gpsPacketTypesNeeded();

    /**
     * @brief Write the information for the gps packet
     */
    void writeGpsPacket(QDataStream &stream, CT_LasPointInfo *info);

private:
    char  m_emptyData[8];
};

#endif // CT_LASGPSPACKET_H
