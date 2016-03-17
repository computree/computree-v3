#ifndef CT_LASPOINTFORMAT6_H
#define CT_LASPOINTFORMAT6_H

#include "abstract/ct_abstractlaspointformat.h"

class CTLIBLAS_EXPORT CT_LASPointFormat6 : public CT_AbstractLASPointFormat
{
public:
    CT_LASPointFormat6();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex);

private:
    char                                    m_emptyData[30];
    qint32                                  m_x, m_y, m_z;
    CT_AbstractCoordinateSystem::realEx     m_xc, m_yc, m_zc;

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();
};

#endif // CT_LASPOINTFORMAT6_H
