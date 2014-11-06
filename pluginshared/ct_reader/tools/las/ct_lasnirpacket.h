#ifndef CT_LASNIRPACKET_H
#define CT_LASNIRPACKET_H

#include "ct_reader/tools/las/ct_lasdefine.h"
#include "ct_reader/tools/las/ct_laspointinfo.h"

#include <QDataStream>

class PLUGINSHAREDSHARED_EXPORT CT_LASNirPacket
{
public:
    CT_LASNirPacket();

    /**
     * @brief Return the size of the nir packet in bytes
     */
    size_t nirPacketSizeInBytes() const;

    /**
     * @brief Returns the types needed to write the nir packet
     */
    QList<CT_LasDefine::LASPointAttributesType> nirPacketTypesNeeded();

    /**
     * @brief Write the information for the nir packet
     */
    void writeNirPacket(QDataStream &stream, CT_LasPointInfo *info);

private:
    char  m_emptyData[2];
};

#endif // CT_LASNIRPACKET_H
