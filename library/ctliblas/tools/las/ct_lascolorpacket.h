#ifndef CT_LASCOLORPACKET_H
#define CT_LASCOLORPACKET_H

#include "ct_lasdefine.h"
#include "ct_laspointinfo.h"

#include <QDataStream>

class CTLIBLAS_EXPORT CT_LASColorPacket
{
public:
    CT_LASColorPacket();

    /**
     * @brief Return the size of the color packet in bytes
     */
    size_t colorPacketSizeInBytes() const;

    /**
     * @brief Returns the types needed to write the color packet
     */
    QList<CT_LasDefine::LASPointAttributesType> colorPacketTypesNeeded();

    /**
     * @brief Write the information for the color packet
     */
    void writeColorPacket(QDataStream &stream, CT_LasPointInfo *info);

private:
    char  m_emptyData[6];
};

#endif // CT_LASCOLORPACKET_H
