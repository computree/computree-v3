#ifndef CT_LASHEADER_H
#define CT_LASHEADER_H

#include "ctliblas/ctliblas_global.h"

#include "ct_itemdrawable/ct_fileheader.h"

#include <QDataStream>

class CTLIBLAS_EXPORT CT_LASHeader : public CT_FileHeader
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_LASHeader, CT_FileHeader, LAS Header)

public:
    CT_LASHeader();

    CT_LASHeader(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result);

    CT_LASHeader(const QString &modelName,
                     const CT_AbstractResult *result);

    virtual bool hasBoundingBox() const {return true;}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);


    char        m_fileSignature[4];
    quint16     m_fileSourceID;
    quint16     m_globalEncoding;
    quint32     m_projectIDGuidData1;
    quint16     m_projectIDGuidData2;
    quint16     m_projectIDGuidData3;
    quint64     m_projectIDGuidData4;
    quint8      m_versionMajor;
    quint8      m_versionMinor;
    char        m_systemID[32];
    char        m_sofwareID[32];
    quint16     m_fileCreationDayOfYear;
    quint16     m_fileCreationYear;
    quint16     m_headerSize;
    quint32     m_offsetToPointData;
    quint32     m_numberOfVariableLengthRecords;
    quint8      m_pointDataRecordFormat;
    quint16     m_pointDataRecordLength;
    quint32     m_legacyNumberOfPointRecord;
    quint32     m_legacyNumberOfPointsByReturn[5];
    double      m_xScaleFactor;
    double      m_yScaleFactor;
    double      m_zScaleFactor;
    double      m_xOffset;
    double      m_yOffset;
    double      m_zOffset;
    quint64     m_startOfWaveformDataPacketRecord;
    quint64     m_startOfFirstExtendedVariableLengthRecord;
    quint32     m_numberOfExtendedVariableLengthRecords;
    quint64     m_numberOfPointRecords;
    quint64     m_numberOfPointsByReturn[15];

    /**
     * @brief Returns the size in bytes
     */
    size_t sizeInBytes() const;

    /**
     * @brief Returns the number of points to read
     */
    size_t getPointsRecordCount() const;

    /**
     * @brief Returns true if points must be scaled or translated
     */
    bool mustTransformPoints() const;

    /**
     * @brief Transform the point passed in parameter (when you read a point)
     */
    void transformPoint(const qint32 &x, const qint32 &y, const qint32 &z, double &xc, double &yc, double &zc) const;

    /**
     * @brief Transform the point passed in parameter (when you want to write a point)
     */
    void inverseTransformPoint(const double &x, const double &y, const double &z, qint32 &xc, qint32 &yc, qint32 &zc) const;

    /**
     * @brief Read the header
     */
    bool read(QDataStream &stream, QString &error);

    /**
     * @brief Write the header
     */
    bool write(QDataStream &stream, QString &error);

    /**
     * @brief Returns all information of this header in a string
     */
    QString toString() const;


    inline QString get_fileSignature() const {return QString(m_fileSignature);} //
    inline quint16 get_fileSourceID() const {return m_fileSourceID;}
    inline quint16 get_globalEncoding() const {return m_globalEncoding;}
    inline quint32 get_projectIDGuidData1() const {return m_projectIDGuidData1;}
    inline quint16 get_projectIDGuidData2() const {return m_projectIDGuidData2;}
    inline quint16 get_projectIDGuidData3() const {return m_projectIDGuidData3;}
    inline quint64 get_projectIDGuidData4() const {return m_projectIDGuidData4;}
    inline quint16 get_versionMajor() const {return m_versionMajor;}
    inline quint16 get_versionMinor() const {return m_versionMinor;}
    inline QString get_systemID() const {return QString(m_systemID);} //
    inline QString get_sofwareID() const {return QString(m_sofwareID);} //
    inline quint16 get_fileCreationDayOfYear() const {return m_fileCreationDayOfYear;}
    inline quint16 get_fileCreationYear() const {return m_fileCreationYear;}
    inline quint16 get_headerSize() const {return m_headerSize;}
    inline quint32 get_offsetToPointData() const {return m_offsetToPointData;}
    inline quint32 get_numberOfVariableLengthRecords() const {return m_numberOfVariableLengthRecords;}
    inline quint16 get_pointDataRecordFormat() const {return m_pointDataRecordFormat;}
    inline quint16 get_pointDataRecordLength() const {return m_pointDataRecordLength;}
    inline quint32 get_legacyNumberOfPointRecord() const {return m_legacyNumberOfPointRecord;}
    inline QString get_legacyNumberOfPointsByReturn() const {return m_legacyNumberOfPointsByReturn_ToString();} //
    inline double get_xScaleFactor() const {return m_xScaleFactor;}
    inline double get_yScaleFactor() const {return m_yScaleFactor;}
    inline double get_zScaleFactor() const {return m_zScaleFactor;}
    inline double get_xOffset() const {return m_xOffset;}
    inline double get_yOffset() const {return m_yOffset;}
    inline double get_zOffset() const {return m_zOffset;}
    inline double get_maxX() const {return maxX();}
    inline double get_minX() const {return minX();}
    inline double get_maxY() const {return maxY();}
    inline double get_minY() const {return minY();}
    inline double get_maxZ() const {return maxZ();}
    inline double get_minZ() const {return minZ();}
    inline quint64 get_startOfWaveformDataPacketRecord() const {return m_startOfWaveformDataPacketRecord;}
    inline quint64 get_startOfFirstExtendedVariableLengthRecord() const {return m_startOfFirstExtendedVariableLengthRecord;}
    inline quint32 get_numberOfExtendedVariableLengthRecords() const {return m_numberOfExtendedVariableLengthRecords;}
    inline quint64 get_numberOfPointRecords() const {return m_numberOfPointRecords;}
    inline QString get_numberOfPointsByReturn() const {return m_numberOfPointsByReturn_ToString();} //

private:
    QString m_legacyNumberOfPointsByReturn_ToString() const;
    QString m_numberOfPointsByReturn_ToString() const;

    CT_DEFAULT_IA_BEGIN(CT_LASHeader)   
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_fileSignature, QObject::tr("File Signature"), "fi")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_fileSourceID, QObject::tr("File Source ID"), "fsid")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_globalEncoding, QObject::tr("Global encoding"), "ge")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_projectIDGuidData1, QObject::tr("Project ID - GUID data 1"), "guid1")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_projectIDGuidData2, QObject::tr("Project ID - GUID data 2"), "guid2")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_projectIDGuidData3, QObject::tr("Project ID - GUID data 3"), "guid3")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_projectIDGuidData4, QObject::tr("Project ID - GUID data 4"), "guid4")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_versionMajor, QObject::tr("Version Major"), "vma")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_versionMinor, QObject::tr("Version Minor"), "vmi")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_systemID, QObject::tr("System Identifier"), "sid")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_sofwareID, QObject::tr("Generating Software"), "gs")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_fileCreationDayOfYear, QObject::tr("File Creation DoY"), "fcdy")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_fileCreationYear, QObject::tr("File Creation Year"), "fcy")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_headerSize, QObject::tr("Header Size"), "hs")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_offsetToPointData, QObject::tr("Offset to point data"), "otpd")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_numberOfVariableLengthRecords, QObject::tr("Number of Variable Length Records"), "nvlr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_pointDataRecordFormat, QObject::tr("Point Data Record Format"), "pdrf")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_pointDataRecordLength, QObject::tr("Point Data Record Length"), "pdrl")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_legacyNumberOfPointRecord, QObject::tr("Legacy Number of point records"), "lnpr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_legacyNumberOfPointsByReturn, QObject::tr("Legacy Number of points by return"), "lnpbr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_xScaleFactor, QObject::tr("X scale factor"), "xsf")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_yScaleFactor, QObject::tr("Y scale factor"), "ysf")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_zScaleFactor, QObject::tr("Z scale factor"), "zsf")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataX(), &CT_LASHeader::get_xOffset, QObject::tr("X offset"), "xo")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataY(), &CT_LASHeader::get_yOffset, QObject::tr("Y offset"), "yo")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataZ(), &CT_LASHeader::get_zOffset, QObject::tr("Z offset"), "zo")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataX(), &CT_LASHeader::get_maxX, QObject::tr("Max X"), "maxx")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataX(), &CT_LASHeader::get_minX, QObject::tr("Min X"), "minx")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataY(), &CT_LASHeader::get_maxY, QObject::tr("Max Y"), "maxy")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataY(), &CT_LASHeader::get_minY, QObject::tr("Min Y"), "miny")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataZ(), &CT_LASHeader::get_maxZ, QObject::tr("Max Z"), "maxz")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataZ(), &CT_LASHeader::get_minZ, QObject::tr("Min Z"), "minz")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_startOfWaveformDataPacketRecord, QObject::tr("Start of Waveform Data Packet Record"), "swdpr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_startOfFirstExtendedVariableLengthRecord, QObject::tr("Start of first Extended Variable Length Record"), "sfevlr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_numberOfExtendedVariableLengthRecords, QObject::tr("Number of Extended Variable Length Records"), "nevlr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_numberOfPointRecords, QObject::tr("Number of point records"), "nopr")
    CT_DEFAULT_IA_V3(CT_LASHeader, CT_AbstractCategory::staticInitDataValue(), &CT_LASHeader::get_numberOfPointsByReturn, QObject::tr("Number of points by return"), "nopbr")
    CT_DEFAULT_IA_END(CT_LASHeader)

};

#endif // CT_LASHEADER_H
