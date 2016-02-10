#ifndef CT_ABSTRACTLASPOINTFORMAT_H
#define CT_ABSTRACTLASPOINTFORMAT_H

#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

#include "ctlibio/readers/ct_reader_las.h"
#include "ctlibio/tools/las/ct_laspointinfo.h"

#define CT_WRITE_LAS_SCALAR(information, type) if(information.first == NULL) { stream << (type)0; } else { stream << (type)information.first->dValueAt(information.second); }

/**
 * @brief A tool class that can be use to write a point in a LAS File
 */
class CTLIBIO_EXPORT CT_AbstractLASPointFormat
{
public:
    CT_AbstractLASPointFormat();
    virtual ~CT_AbstractLASPointFormat();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const = 0;

    /**
     * @brief Set LAS Header
     */
    virtual void setHeader(const CT_LASHeader *header);

    /**
     * @brief LAS Header used
     */
    inline CT_LASHeader* header() const { return m_lasHeader; }

    /**
     * @brief Set attributes available to export a point
     */
    virtual void setAttributes(const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*> &attributes);

    /**
     * @brief Return attributes used to write the point
     */
    const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*>& attributes() const;

    /**
     * @brief Call this method before use the method "write" and after "setAttributes". This will create the
     *        informations necessary for each points to write.
     */
    virtual bool initWrite();

    /**
     * @brief Return the info for point at globalIndex
     */
    inline CT_LasPointInfo* infoOfPoint(const size_t &globalIndex) const { return m_infos.value(globalIndex, NULL); }

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, const CT_Point &p, const size_t &globalIndex) = 0;

private:
    // attributes to be used to write points
    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*>  m_attributes;

    // key = globalIndex of the point, value = information of the point
    QHash<size_t, CT_LasPointInfo*>     m_infos;

    CT_LASHeader                        *m_lasHeader;

protected:

    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch() = 0;

    /**
     * @brief Clear infos of all points
     */
    void clearInfos();

};

#endif // CT_ABSTRACTLASPOINTFORMAT_H
