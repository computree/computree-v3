#ifndef CT_LASPOINTFORMAT10_H
#define CT_LASPOINTFORMAT10_H

#include "ct_laspointformat9.h"
#include "ct_lascolorpacket.h"
#include "ct_lasnirpacket.h"

class CTLIBIO_EXPORT CT_LASPointFormat10 : public CT_LASPointFormat9, public CT_LASColorPacket, public CT_LASNirPacket
{
public:
    CT_LASPointFormat10();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex);

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();

    /**
     * @brief Write only the information of the point format 10
     */
    void writeInfoFormat10(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT10_H
