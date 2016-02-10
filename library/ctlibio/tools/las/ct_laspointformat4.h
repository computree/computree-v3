#ifndef CT_LASPOINTFORMAT4_H
#define CT_LASPOINTFORMAT4_H

#include "ct_laspointformat1.h"
#include "ct_laswavepacket.h"

class CTLIBIO_EXPORT CT_LASPointFormat4 : public CT_LASPointFormat1, public CT_LASWavePacket
{
public:
    CT_LASPointFormat4();

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
     * @brief Write only the information of the point format 4
     */
    void writeInfoFormat4(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT4_H
