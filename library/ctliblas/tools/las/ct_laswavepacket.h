#ifndef CT_LASWAVEPACKET_H
#define CT_LASWAVEPACKET_H

#include "ct_lasdefine.h"
#include "ct_laspointinfo.h"

#include <QDataStream>

class CTLIBLAS_EXPORT CT_LASWavePacket
{
public:
    CT_LASWavePacket();

    /**
     * @brief Return the size of the wave packet in bytes
     */
    size_t wavePacketSizeInBytes() const;

    /**
     * @brief Returns the types needed to write the wave packet
     */
    QList<CT_LasDefine::LASPointAttributesType> wavePacketTypesNeeded();

    /**
     * @brief Write the information for the wave packet
     */
    void writeWavePacket(QDataStream &stream, CT_LasPointInfo *info);

private:
    char  m_emptyData[29];
};

#endif // CT_LASWAVEPACKET_H
