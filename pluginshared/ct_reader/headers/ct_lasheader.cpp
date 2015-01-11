#include "ct_lasheader.h"

#include <QDate>

#define readRawDataAndCheck(Param, N, Err) if(stream.readRawData(Param, N) != N) { error = Err; return false; } else { filePos += N; }
#define readData(Param, Err) if(stream.atEnd()) { error = Err; return false; } else { stream >> Param; filePos += sizeof(Param); }

#define checkHeaderSize if(filePos > m_headerSize) { /*error = QObject::tr("L'élément lu dépasse la taille de l'en-tête défini dans le fichier"); return false;*/ }
#define readRawDataAndCheckSize(Param, N, Err) if(stream.readRawData(Param, N) != N) { error = Err; return false; } else { filePos += N; checkHeaderSize; }
#define readDataAndCheckSize(Param, Err) if(stream.atEnd()) { error = Err; return false; } else { stream >> Param; filePos += sizeof(Param); checkHeaderSize; }

CT_DEFAULT_IA_INIT(CT_LASHeader)

CT_LASHeader::CT_LASHeader() : CT_FileHeader()
{
    for(int i=0; i<4; ++i)
        m_fileSignature[i] = ' ';

    m_fileSourceID = 0;
    m_globalEncoding = 0;

    m_projectIDGuidData1 = 0;
    m_projectIDGuidData2 = 0;
    m_projectIDGuidData3 = 0;
    m_projectIDGuidData4 = 0;

    m_versionMajor = 0;
    m_versionMinor = 0;

    for(int i=0; i<32; ++i)
        m_systemID[i] = ' ';

    for(int i=0; i<32; ++i)
        m_sofwareID[i] = ' ';

    m_fileCreationDayOfYear = 0;
    m_fileCreationYear = 0;
    m_headerSize = 0;
    m_offsetToPointData = 0;
    m_numberOfVariableLengthRecords = 0;
    m_pointDataRecordFormat = 0;
    m_pointDataRecordLength = 0;
    m_legacyNumberOfPointRecord = 0;

    for(int i=0; i<5; ++i)
        m_legacyNumberOfPointsByReturn[i] = 0;

    m_xScaleFactor = 1;
    m_yScaleFactor = 1;
    m_zScaleFactor = 1;
    m_xOffset = 0;
    m_yOffset = 0;
    m_zOffset = 0;
    m_maxX = 0;
    m_minX = 0;
    m_maxY = 0;
    m_minY = 0;
    m_maxZ = 0;
    m_minZ = 0;
    m_startOfWaveformDataPacketRecord = 0;
    m_startOfFirstExtendedVariableLengthRecord = 0;
    m_numberOfExtendedVariableLengthRecords = 0;
    m_numberOfPointRecords = 0;

    for(int i=0; i<15; ++i)
        m_numberOfPointsByReturn[i] = 0;
}

CT_LASHeader::CT_LASHeader(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_FileHeader(model, result)
{
    for(int i=0; i<4; ++i)
        m_fileSignature[i] = ' ';

    m_fileSourceID = 0;
    m_globalEncoding = 0;

    m_projectIDGuidData1 = 0;
    m_projectIDGuidData2 = 0;
    m_projectIDGuidData3 = 0;
    m_projectIDGuidData4 = 0;

    m_versionMajor = 0;
    m_versionMinor = 0;

    for(int i=0; i<32; ++i)
        m_systemID[i] = ' ';

    for(int i=0; i<32; ++i)
        m_sofwareID[i] = ' ';

    m_fileCreationDayOfYear = 0;
    m_fileCreationYear = 0;
    m_headerSize = 0;
    m_offsetToPointData = 0;
    m_numberOfVariableLengthRecords = 0;
    m_pointDataRecordFormat = 0;
    m_pointDataRecordLength = 0;
    m_legacyNumberOfPointRecord = 0;

    for(int i=0; i<5; ++i)
        m_legacyNumberOfPointsByReturn[i] = 0;

    m_xScaleFactor = 1;
    m_yScaleFactor = 1;
    m_zScaleFactor = 1;
    m_xOffset = 0;
    m_yOffset = 0;
    m_zOffset = 0;
    m_maxX = 0;
    m_minX = 0;
    m_maxY = 0;
    m_minY = 0;
    m_maxZ = 0;
    m_minZ = 0;
    m_startOfWaveformDataPacketRecord = 0;
    m_startOfFirstExtendedVariableLengthRecord = 0;
    m_numberOfExtendedVariableLengthRecords = 0;
    m_numberOfPointRecords = 0;

    for(int i=0; i<15; ++i)
        m_numberOfPointsByReturn[i] = 0;
}

CT_LASHeader::CT_LASHeader(const QString &modelName, const CT_AbstractResult *result) : CT_FileHeader(modelName, result)
{
    for(int i=0; i<4; ++i)
        m_fileSignature[i] = ' ';

    m_fileSourceID = 0;
    m_globalEncoding = 0;

    m_projectIDGuidData1 = 0;
    m_projectIDGuidData2 = 0;
    m_projectIDGuidData3 = 0;
    m_projectIDGuidData4 = 0;

    m_versionMajor = 0;
    m_versionMinor = 0;

    for(int i=0; i<32; ++i)
        m_systemID[i] = ' ';

    for(int i=0; i<32; ++i)
        m_sofwareID[i] = ' ';

    m_fileCreationDayOfYear = 0;
    m_fileCreationYear = 0;
    m_headerSize = 0;
    m_offsetToPointData = 0;
    m_numberOfVariableLengthRecords = 0;
    m_pointDataRecordFormat = 0;
    m_pointDataRecordLength = 0;
    m_legacyNumberOfPointRecord = 0;

    for(int i=0; i<5; ++i)
        m_legacyNumberOfPointsByReturn[i] = 0;

    m_xScaleFactor = 1;
    m_yScaleFactor = 1;
    m_zScaleFactor = 1;
    m_xOffset = 0;
    m_yOffset = 0;
    m_zOffset = 0;
    m_maxX = 0;
    m_minX = 0;
    m_maxY = 0;
    m_minY = 0;
    m_maxZ = 0;
    m_minZ = 0;
    m_startOfWaveformDataPacketRecord = 0;
    m_startOfFirstExtendedVariableLengthRecord = 0;
    m_numberOfExtendedVariableLengthRecords = 0;
    m_numberOfPointRecords = 0;

    for(int i=0; i<15; ++i)
        m_numberOfPointsByReturn[i] = 0;
}

QString CT_LASHeader::getType() const
{
    return staticGetType();
}

QString CT_LASHeader::staticGetType()
{
    return CT_FileHeader::staticGetType() + "/CT_LASHeader";
}

CT_AbstractItemDrawable *CT_LASHeader::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_LASHeader *cpy =  new CT_LASHeader((CT_OutAbstractSingularItemModel*) model, result);
    cpy->setFile(_fileInfo.filePath());

    for (int i = 0 ; i < 4 ; i++) {cpy->m_fileSignature[i] = m_fileSignature[i];}
    cpy->m_fileSourceID = m_fileSourceID;
    cpy->m_globalEncoding = m_globalEncoding;
    cpy->m_projectIDGuidData1 = m_projectIDGuidData1;
    cpy->m_projectIDGuidData2 = m_projectIDGuidData2;
    cpy->m_projectIDGuidData3 = m_projectIDGuidData3;
    cpy->m_projectIDGuidData4 = m_projectIDGuidData4;
    cpy->m_versionMajor = m_versionMajor;
    cpy->m_versionMinor = m_versionMinor;
    for (int i = 0 ; i < 32 ; i++) {cpy->m_systemID[i] = m_systemID[i];}
    for (int i = 0 ; i < 32 ; i++) {cpy->m_sofwareID[i] = m_sofwareID[i];}
    cpy->m_fileCreationDayOfYear = m_fileCreationDayOfYear;
    cpy->m_fileCreationYear = m_fileCreationYear;
    cpy->m_headerSize = m_headerSize;
    cpy->m_offsetToPointData = m_offsetToPointData;
    cpy->m_numberOfVariableLengthRecords = m_numberOfVariableLengthRecords;
    cpy->m_pointDataRecordFormat = m_pointDataRecordFormat;
    cpy->m_pointDataRecordLength = m_pointDataRecordLength;
    cpy->m_legacyNumberOfPointRecord = m_legacyNumberOfPointRecord;
    for (int i = 0 ; i < 5 ; i++) {cpy->m_legacyNumberOfPointsByReturn[i] = m_legacyNumberOfPointsByReturn[i];}
    cpy->m_xScaleFactor = m_xScaleFactor;
    cpy->m_yScaleFactor = m_yScaleFactor;
    cpy->m_zScaleFactor = m_zScaleFactor;
    cpy->m_xOffset = m_xOffset;
    cpy->m_yOffset = m_yOffset;
    cpy->m_zOffset = m_zOffset;
    cpy->m_maxX = m_maxX;
    cpy->m_minX = m_minX;
    cpy->m_maxY = m_maxY;
    cpy->m_minY = m_minY;
    cpy->m_maxZ = m_maxZ;
    cpy->m_minZ = m_minZ;
    cpy->m_startOfWaveformDataPacketRecord = m_startOfWaveformDataPacketRecord;
    cpy->m_startOfFirstExtendedVariableLengthRecord = m_startOfFirstExtendedVariableLengthRecord;
    cpy->m_numberOfExtendedVariableLengthRecords = m_numberOfExtendedVariableLengthRecords;
    cpy->m_numberOfPointRecords = m_numberOfPointRecords;
    for (int i = 0 ; i < 15 ; i++) {cpy->m_numberOfPointsByReturn[i] = m_numberOfPointsByReturn[i];}

    return cpy;
}


size_t CT_LASHeader::sizeInBytes() const
{
    return sizeof(m_fileSignature) +
            sizeof(m_fileSourceID) +
            sizeof(m_globalEncoding) +
            sizeof(m_projectIDGuidData1) +
            sizeof(m_projectIDGuidData2) +
            sizeof(m_projectIDGuidData3) +
            sizeof(m_projectIDGuidData4) +
            sizeof(m_versionMajor) +
            sizeof(m_versionMinor) +
            sizeof(m_systemID) +
            sizeof(m_sofwareID) +
            sizeof(m_fileCreationDayOfYear) +
            sizeof(m_fileCreationYear) +
            sizeof(m_headerSize) +
            sizeof(m_offsetToPointData) +
            sizeof(m_numberOfVariableLengthRecords) +
            sizeof(m_pointDataRecordFormat) +
            sizeof(m_pointDataRecordLength) +
            sizeof(m_legacyNumberOfPointRecord) +
            sizeof(m_legacyNumberOfPointsByReturn) +
            sizeof(m_xScaleFactor) +
            sizeof(m_yScaleFactor) +
            sizeof(m_zScaleFactor) +
            sizeof(m_xOffset) +
            sizeof(m_yOffset) +
            sizeof(m_zOffset) +
            sizeof(m_maxX) +
            sizeof(m_minX) +
            sizeof(m_maxY) +
            sizeof(m_minY) +
            sizeof(m_maxZ) +
            sizeof(m_minZ) +
            sizeof(m_startOfWaveformDataPacketRecord) +
            sizeof(m_startOfFirstExtendedVariableLengthRecord) +
            sizeof(m_numberOfExtendedVariableLengthRecords) +
            sizeof(m_numberOfPointRecords) +
            sizeof(m_numberOfPointsByReturn);
}

size_t CT_LASHeader::getPointsRecordCount() const
{
    if(m_legacyNumberOfPointRecord == 0)
        return m_numberOfPointRecords;

    return m_legacyNumberOfPointRecord;
}

bool CT_LASHeader::mustTransformPoints() const
{
    return (m_xOffset != 0) || (m_yOffset != 0) || (m_zOffset != 0) || (m_xScaleFactor != 1) || (m_yScaleFactor != 1) || (m_zScaleFactor != 1);
}

void CT_LASHeader::transformPoint(const qint32 &x, const qint32 &y, const qint32 &z, double &xc, double &yc, double &zc) const
{
    xc = (((double)x)*m_xScaleFactor) + m_xOffset;
    yc = (((double)y)*m_yScaleFactor) + m_yOffset;
    zc = (((double)z)*m_zScaleFactor) + m_zOffset;
}

void CT_LASHeader::inverseTransformPoint(const double &x, const double &y, const double &z, qint32 &xc, qint32 &yc, qint32 &zc) const
{
    xc = ((double)(x - m_xOffset))/m_xScaleFactor;
    yc = ((double)(y - m_yOffset))/m_yScaleFactor;
    zc = ((double)(z - m_zOffset))/m_zScaleFactor;
}

bool CT_LASHeader::read(QDataStream &stream, QString &error)
{
    error = "";

    stream.device()->seek(0);
    stream.setByteOrder(QDataStream::LittleEndian);

    // used in #define (readRawDataAndCheck and readData)
    size_t filePos = 0;

    // File Signature
    readRawDataAndCheck(m_fileSignature, 4, QObject::tr("File Signature invalid"));

    if((m_fileSignature[0] != 'L')
            || (m_fileSignature[1] != 'A')
            || (m_fileSignature[2] != 'S')
            || (m_fileSignature[3] != 'F')) {
        error = QObject::tr("Not a LAS File");
        return NULL;
    }

    // File Source ID
    readData(m_fileSourceID, QObject::tr("File Source ID invalid"));

    // Global Encoding
    readData(m_globalEncoding, QObject::tr("Global Encoding invalid"));

    // Project ID
    readData(m_projectIDGuidData1, QObject::tr("Project ID 1 invalid"));
    readData(m_projectIDGuidData2, QObject::tr("Project ID 2 invalid"));
    readData(m_projectIDGuidData3, QObject::tr("Project ID 3 invalid"));
    readData(m_projectIDGuidData4, QObject::tr("Project ID 4 invalid"));

    // Version Major
    readData(m_versionMajor, QObject::tr("Version Major invalid"));

    // Version Minor
    readData(m_versionMinor, QObject::tr("Version Minor invalid"));

    // System Identifier
    readRawDataAndCheck(m_systemID, 32, QObject::tr("System Identifier invalid"));

    // Sofware Identifier
    readRawDataAndCheck(m_sofwareID, 32, QObject::tr("Sofware Identifier invalid"));

    // File Creation Day of Year
    readData(m_fileCreationDayOfYear, QObject::tr("File Creation Day of Year invalid"));

    // File Creation Year
    readData(m_fileCreationYear, QObject::tr("File Creation Year invalid"));

    // Header Size
    readData(m_headerSize, QObject::tr("Header Size invalid"));

    // Offset To Point Data
    readData(m_offsetToPointData, QObject::tr("Offset To Point Data invalid"));

    if(m_offsetToPointData < m_headerSize) {

        error = QObject::tr("The offset to the start of points data (%1) is smaller than the header size (%2).").arg(m_offsetToPointData).arg(m_headerSize);
        return NULL;
    }

    // Number of Variable Length Records
    readData(m_numberOfVariableLengthRecords, QObject::tr("Number of Variable Length Records invalid"));

    // Point Data Record Format
    readData(m_pointDataRecordFormat, QObject::tr("Point Data Record Format invalid"));

    // Point Data Record Length
    readData(m_pointDataRecordLength, QObject::tr("Point Data Record Length invalid"));

    // Legacy Number of point records
    readData(m_legacyNumberOfPointRecord, QObject::tr("Legacy Number of point records invalid"));

    // Legacy Number of points by return
    for(int i=0; i<5; ++i) {
        readData(m_legacyNumberOfPointsByReturn[i], QObject::tr("Legacy Number of points by return (%1) invalid").arg(i));
    }

    readData(m_xScaleFactor, QObject::tr("X Scale Factor invalid"));
    readData(m_yScaleFactor, QObject::tr("Y Scale Factor invalid"));
    readData(m_zScaleFactor, QObject::tr("Z Scale Factor invalid"));
    readData(m_xOffset, QObject::tr("X Offset invalid"));
    readData(m_yOffset, QObject::tr("Y Offset invalid"));
    readData(m_zOffset, QObject::tr("Z Offset invalid"));
    readData(m_maxX, QObject::tr("Max X invalid"));
    readData(m_minX, QObject::tr("Min X invalid"));
    readData(m_maxY, QObject::tr("Max Y invalid"));
    readData(m_minY, QObject::tr("Min Y invalid"));
    readData(m_maxZ, QObject::tr("Max Z invalid"));
    readData(m_minZ, QObject::tr("Min Z invalid"));

    float x, y, z;
    PS_COORDINATES_SYS->convertImport(m_minX, m_minY, m_minZ, x, y, z);
    _minCoordinates(0) = x;
    _minCoordinates(1) = y;
    _minCoordinates(2) = z;
    PS_COORDINATES_SYS->convertImport(m_maxX, m_maxY, m_maxZ, x, y, z);
    _maxCoordinates(0) = x;
    _maxCoordinates(1) = y;
    _maxCoordinates(2) = z;

    setCenterX((maxX() + minX())/2.0);
    setCenterY((maxY() + minY())/2.0);
    setCenterZ((maxZ() + minZ())/2.0);

    readDataAndCheckSize(m_startOfWaveformDataPacketRecord, QObject::tr("Start of Waveform Data Packet Record invalid"));
    readDataAndCheckSize(m_startOfFirstExtendedVariableLengthRecord, QObject::tr("Start Of First Extended Variable Length Record invalid"));
    readDataAndCheckSize(m_numberOfExtendedVariableLengthRecords, QObject::tr("Number Of Extended Variable Length Records invalid"));
    readDataAndCheckSize(m_numberOfPointRecords, QObject::tr("Number Of Point Records invalid"));

    for(int i=0; i<15; ++i) {
        quint64 p;
        readDataAndCheckSize(p, QObject::tr("Number Of Points By Return (%1) invalid").arg(i));
        m_numberOfPointsByReturn[i] = p;
    }

    return true;
}

bool CT_LASHeader::write(QDataStream &stream, QString &error)
{
    error = "";

    stream.device()->seek(0);
    stream.setByteOrder(QDataStream::LittleEndian);

    // File Signature
    stream.writeRawData("LASF", 4);

    // File Source ID
    stream << m_fileSourceID;

    // Global Encoding
    stream << m_globalEncoding;

    // Project ID
    stream << m_projectIDGuidData1;
    stream << m_projectIDGuidData2;
    stream << m_projectIDGuidData3;
    stream << m_projectIDGuidData4;

    // Version Major
    stream << m_versionMajor;

    // Version Minor
    stream << m_versionMinor;

    // System Identifier
    stream.writeRawData(m_systemID, sizeof(m_systemID));

    // Sofware Identifier
    stream.writeRawData(m_sofwareID, sizeof(m_sofwareID));

    // File Creation Day of Year
    stream << m_fileCreationDayOfYear;

    // File Creation Year
    stream << m_fileCreationYear;

    m_headerSize = sizeInBytes();
    m_offsetToPointData = m_headerSize;

    // Header Size
    stream << m_headerSize;

    // Offset To Point Data
    stream << m_offsetToPointData;

    // Number of Variable Length Records
    stream << m_numberOfVariableLengthRecords;

    // Point Data Record Format
    stream << m_pointDataRecordFormat;

    // Point Data Record Length
    stream << m_pointDataRecordLength;

    // Legacy Number of point records
    stream << m_legacyNumberOfPointRecord;

    // Legacy Number of points by return
    for(int i=0; i<5; ++i) {
        stream << m_legacyNumberOfPointsByReturn[i];
    }

    stream << m_xScaleFactor;
    stream << m_yScaleFactor;
    stream << m_zScaleFactor;
    stream << m_xOffset;
    stream << m_yOffset;
    stream << m_zOffset;
    stream << m_maxX;
    stream << m_minX;
    stream << m_maxY;
    stream << m_minY;
    stream << m_maxZ;
    stream << m_minZ;
    stream << m_startOfWaveformDataPacketRecord;
    stream << m_startOfFirstExtendedVariableLengthRecord;
    stream << m_numberOfExtendedVariableLengthRecords;
    stream << m_numberOfPointRecords;

    for(int i=0; i<15; ++i) {
        stream << m_numberOfPointsByReturn[i];
    }

    return true;
}

QString CT_LASHeader::toString() const
{
    QString str;

    QDate date(1,1,1);
    date = date.addYears(m_fileCreationYear-1);
    date = date.addDays(m_fileCreationDayOfYear-1);

    str += QObject::tr("File Signature : %1").arg(m_fileSignature) + "\r\n";
    str += QObject::tr("File Source ID : %1").arg(m_fileSourceID) + "\r\n";
    str += QObject::tr("Project ID - GUID data 1 : %1").arg(m_projectIDGuidData1) + "\r\n";
    str += QObject::tr("Project ID - GUID data 2 : %1").arg(m_projectIDGuidData2) + "\r\n";
    str += QObject::tr("Project ID - GUID data 3 : %1").arg(m_projectIDGuidData3) + "\r\n";
    str += QObject::tr("Project ID - GUID data 4 : %1").arg(m_projectIDGuidData4) + "\r\n";
    str += QObject::tr("Version Major : %1").arg(m_versionMajor) + "\r\n";
    str += QObject::tr("Version Minor : %1").arg(m_versionMinor) + "\r\n";
    str += QObject::tr("System Identifier : %1").arg(m_systemID) + "\r\n";
    str += QObject::tr("Generating Software : %1").arg(m_sofwareID) + "\r\n";
    str += QObject::tr("File Creation : %1").arg(date.toString(QObject::tr("dd/MM/yyyy"))) + "\r\n";
    str += QObject::tr("Header Size : %1").arg(m_headerSize) + "\r\n";
    str += QObject::tr("Offset to point data : %1").arg(m_offsetToPointData) + "\r\n";
    str += QObject::tr("Number of Variable Length Records : %1").arg(m_numberOfVariableLengthRecords) + "\r\n";
    str += QObject::tr("Point Data Record Format : %1").arg(m_pointDataRecordFormat) + "\r\n";
    str += QObject::tr("Point Data Record Length : %1").arg(m_pointDataRecordLength) + "\r\n";
    str += QObject::tr("Legacy Number of point records : %1").arg(m_legacyNumberOfPointRecord) + "\r\n";
    str += QObject::tr("Legacy Number of points by return : %1 / ...").arg(m_legacyNumberOfPointsByReturn[0]) + "\r\n";
    str += QObject::tr("X/Y/Z scale factor : %1 / %2 / %3").arg(m_xScaleFactor).arg(m_yScaleFactor).arg(m_zScaleFactor) + "\r\n";
    str += QObject::tr("X/Y/Z offset : %1 / %2 / %3").arg(m_xOffset).arg(m_yOffset).arg(m_zOffset) + "\r\n";
    str += QObject::tr("Min X/Y/Z : %1 / %2 / %3").arg(m_minX).arg(m_minY).arg(m_minZ) + "\r\n";
    str += QObject::tr("Max X/Y/Z : %1 / %2 / %3").arg(m_maxX).arg(m_maxY).arg(m_maxZ) + "\r\n";
    str += QObject::tr("Start of Waveform Data Packet Record : %1").arg(m_startOfWaveformDataPacketRecord) + "\r\n";
    str += QObject::tr("Start of first Extended Variable Length Record : %1").arg(m_startOfFirstExtendedVariableLengthRecord) + "\r\n";
    str += QObject::tr("Number of Extended Variable Length Records : %1").arg(m_numberOfExtendedVariableLengthRecords) + "\r\n";
    str += QObject::tr("Number of point records : %1").arg(m_numberOfPointRecords) + "\r\n";
    str += QObject::tr("Number of points by return : %1 / ...").arg(m_numberOfPointsByReturn[0]) + "\r\n";

    return str;
}

QString CT_LASHeader::m_legacyNumberOfPointsByReturn_ToString() const
{
    QString str = "";
    for (int i = 0 ; i < 5 ; i++)
    {
        str.append(QString::number(m_legacyNumberOfPointsByReturn[i]));
        if (i < 4) {str.append(" ");}
    }
    return str;
}

QString CT_LASHeader::m_numberOfPointsByReturn_ToString() const
{
    QString str = "";
    for (int i = 0 ; i < 15 ; i++)
    {
        str.append(QString::number(m_numberOfPointsByReturn[i]));
        if (i < 14) {str.append(" ");}
    }
    return str;
}
