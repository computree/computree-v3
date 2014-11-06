#ifndef CT_LASPOINTFORMAT1_H
#define CT_LASPOINTFORMAT1_H

#include "ct_reader/tools/las/ct_laspointformat0.h"
#include "ct_reader/tools/las/ct_lasgpspacket.h"

class PLUGINSHAREDSHARED_EXPORT CT_LASPointFormat1 : public CT_LASPointFormat0, public CT_LASGpsPacket
{
public:
    CT_LASPointFormat1();

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
     * @brief Write only the information of the point format 1
     */
    void writeInfoFormat1(QDataStream &stream, CT_LasPointInfo *info);
};

#endif // CT_LASPOINTFORMAT1_H
