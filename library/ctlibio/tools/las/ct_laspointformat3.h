#ifndef CT_LASPOINTFORMAT3_H
#define CT_LASPOINTFORMAT3_H

#include "ct_laspointformat2.h"
#include "ct_lasgpspacket.h"

class CTLIBIO_EXPORT CT_LASPointFormat3 : public CT_LASPointFormat2, public CT_LASGpsPacket
{
public:
    CT_LASPointFormat3();

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
     * @brief Write only the information of the point format 3
     */
    void writeInfoFormat3(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT3_H
