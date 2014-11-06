#ifndef CT_LASHEADER_H
#define CT_LASHEADER_H

#include "pluginShared_global.h"

#include <QDataStream>

class PLUGINSHAREDSHARED_EXPORT CT_LASHeader
{
public:
    CT_LASHeader();

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
    double      m_maxX;
    double      m_minX;
    double      m_maxY;
    double      m_minY;
    double      m_maxZ;
    double      m_minZ;
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
};

#endif // CT_LASHEADER_H
