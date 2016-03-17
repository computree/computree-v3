#ifndef CT_LASPOINTFORMAT7_H
#define CT_LASPOINTFORMAT7_H

#include "ct_laspointformat6.h"
#include "ct_lascolorpacket.h"

class CTLIBLAS_EXPORT CT_LASPointFormat7 : public CT_LASPointFormat6, public CT_LASColorPacket
{
public:
    CT_LASPointFormat7();

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
     * @brief Write only the information of the point format 7
     */
    void writeInfoFormat7(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT7_H
