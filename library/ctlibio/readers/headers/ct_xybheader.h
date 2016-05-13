#ifndef CT_XYBHEADER_H
#define CT_XYBHEADER_H

#include "ctlibio/ctlibio_global.h"
#include "ct_itemdrawable/ct_fileheader.h"


class CTLIBIO_EXPORT CT_XYBHeader : public CT_FileHeader
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_XYBHeader, CT_FileHeader)

public:
    CT_XYBHeader();

    CT_XYBHeader(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result);

    CT_XYBHeader(const QString &modelName,
                     const CT_AbstractResult *result);

    virtual bool hasBoundingBox() const {return false;}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    inline double get_scanCenterX() const {return m_scanCenterX;}
    inline double get_scanCenterY() const {return m_scanCenterY;}
    inline double get_scanCenterZ() const {return m_scanCenterZ;}
    inline int get_nrows() const {return m_nrows;}
    inline int get_cols() const {return m_ncols;}

    double        m_scanCenterX;
    double        m_scanCenterY;
    double        m_scanCenterZ;
    int        m_nrows;
    int        m_ncols;

private:

    CT_DEFAULT_IA_BEGIN(CT_XYBHeader)
    CT_DEFAULT_IA_V3(CT_XYBHeader, CT_AbstractCategory::staticInitDataValue(), &CT_XYBHeader::get_scanCenterX, QObject::tr("Scan Center X"), "scX")
    CT_DEFAULT_IA_V3(CT_XYBHeader, CT_AbstractCategory::staticInitDataValue(), &CT_XYBHeader::get_scanCenterY, QObject::tr("Scan Center Y"), "scY")
    CT_DEFAULT_IA_V3(CT_XYBHeader, CT_AbstractCategory::staticInitDataValue(), &CT_XYBHeader::get_scanCenterZ, QObject::tr("Scan Center Z"), "scZ")
    CT_DEFAULT_IA_V3(CT_XYBHeader, CT_AbstractCategory::staticInitDataValue(), &CT_XYBHeader::get_nrows, QObject::tr("Scan rows number"), "scR")
    CT_DEFAULT_IA_V3(CT_XYBHeader, CT_AbstractCategory::staticInitDataValue(), &CT_XYBHeader::get_cols, QObject::tr("Scan columns number"), "scC")
    CT_DEFAULT_IA_END(CT_XYBHeader)

};

#endif // CT_XYBHeader_H
