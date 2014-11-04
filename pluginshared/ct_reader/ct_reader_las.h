#ifndef CT_READER_LAS_H
#define CT_READER_LAS_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ct_reader/ct_reader_las_def_models.h"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_LAS_Header
{
public:
    CT_Reader_LAS_Header();

    QString     m_fileSignature;
    quint16     m_fileSourceID;
    quint16     m_globalEncoding;
    quint8      m_projectID[16];
    quint8      m_versionMajor;
    quint8      m_versionMinor;
    QString     m_systemID;
    QString     m_sofwareID;
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
     * @brief Returns the number of points to read
     */
    size_t getPointsRecordCount() const;

    /**
     * @brief Returns true if points must be scaled or translated
     */
    bool mustTransformPoints() const;

    /**
     * @brief Transform the point passed in parameter
     */
    void transformPoint(const qint32 &x, const qint32 &y, const qint32 &z, double &xc, double &yc, double &zc) const;
};

/**
 * @brief Read LAS File (http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html)
 */
class PLUGINSHAREDSHARED_EXPORT CT_Reader_LAS : public CT_AbstractReader
{
    Q_OBJECT

public:

    struct PointCore0_5
    { 
        quint8  entire; // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Number of Returns (3 bits) - Return Number (3 bits)

        typedef quint8 MASK;
    };

    struct PointCore6_10
    {
        quint16 entire; // Number of Returns (4 bits) - Return Number (4 bits)
                        // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Scanner Channel (2 bits) - Classification Flags (4 bits)

        typedef quint16 MASK;
    };

    CT_Reader_LAS();
    virtual ~CT_Reader_LAS();

    bool setFilePath(const QString &filepath);

    //bool configure();

    /**
     * @brief Read the header and return it if it was no error. Otherwise return NULL.
     *        You are responsible to delete the header returned by this method.
     */
    CT_Reader_LAS_Header* readHeader(QString &error) const;

    CT_AbstractReader* copy() const;

protected:

    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    /**
     * @brief Read the header of the file
     */
    CT_Reader_LAS_Header* readHeader(QDataStream &stream, QString &error) const;

private:
    CT_Reader_LAS_Header   *m_header;
    bool            m_centerCloud;
};

#endif // CT_READER_LAS_H
