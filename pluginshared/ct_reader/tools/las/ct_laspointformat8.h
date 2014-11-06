#ifndef CT_LASPOINTFORMAT8_H
#define CT_LASPOINTFORMAT8_H

#include "ct_reader/tools/las/ct_laspointformat7.h"
#include "ct_reader/tools/las/ct_lasnirpacket.h"

class PLUGINSHAREDSHARED_EXPORT CT_LASPointFormat8 : public CT_LASPointFormat7, public CT_LASNirPacket
{
public:
    CT_LASPointFormat8();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const size_t &globalIndex);

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();

    /**
     * @brief Write only the information of the point format 8
     */
    void writeInfoFormat8(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT8_H
