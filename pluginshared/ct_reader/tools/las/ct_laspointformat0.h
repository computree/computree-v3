#ifndef CT_LASPOINTFORMAT0_H
#define CT_LASPOINTFORMAT0_H

#include "ct_reader/tools/las/abstract/ct_abstractlaspointformat.h"

/**
 * @brief A tool class that can be use to write a point of format 0 in a LAS File
 */
class PLUGINSHAREDSHARED_EXPORT CT_LASPointFormat0 : public CT_AbstractLASPointFormat
{
public:
    CT_LASPointFormat0();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex);

private:
    char                                    m_emptyData[20];
    qint32                                  m_x, m_y, m_z;
    CT_AbstractCoordinateSystem::realEx     m_xc, m_yc, m_zc;

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();
};

#endif // CT_LASPOINTFORMAT0_H
