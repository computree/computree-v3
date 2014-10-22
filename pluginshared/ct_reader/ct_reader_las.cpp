#include "ct_reader_las.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"

#include <QMessageBox>

#include <QFile>
#include <QDataStream>

#define readRawData(Param, N, Err) if(stream.readRawData(Param, N) != N) { error = Err; return NULL; } else { filePos += N; }
#define readData(Param, Err) if(stream.atEnd()) { error = Err; return NULL; } else { stream >> Param; filePos += sizeof(Param); }

#define checkHeaderSize if(filePos >= header.m_headerSize) { new CT_Reader_LAS_Header(header); }
#define readRawDataAndCheckSize(Param, N, Err) if(stream.readRawData(Param, N) != N) { error = Err; return NULL; } else { filePos += N; checkHeaderSize; }
#define readDataAndCheckSize(Param, Err) if(stream.atEnd()) { error = Err; return NULL; } else { stream >> Param; filePos += sizeof(Param); checkHeaderSize; }

CT_Reader_LAS::CT_Reader_LAS()
{
    m_header = NULL;
    m_centerCloud = false;
}

CT_Reader_LAS::~CT_Reader_LAS()
{
    delete m_header;
}

bool CT_Reader_LAS::setFilePath(const QString &filepath)
{
    bool ok = false;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);
            QString error;

            CT_Reader_LAS_Header *header = readHeader(stream, error);

            if(header != NULL)
                ok = CT_AbstractReader::setFilePath(filepath);

            if(ok)
                m_header = header;
            else
                delete header;
        }

        f.close();
    }

    return ok;
}

CT_Reader_LAS_Header* CT_Reader_LAS::readHeader(QString &error) const
{
    CT_Reader_LAS_Header *header = NULL;

    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);

            header = readHeader(stream, error);
        }

        f.close();
    }

    return header;
}

/*bool CT_Reader_LAS::configure()
{
    m_centerCloud = QMessageBox::question(NULL, tr("Question"), tr("Voulez vous centrer le nuage de points ?")) == QMessageBox::Yes;

    return true;
}*/

CT_AbstractReader* CT_Reader_LAS::copy() const
{
    return new CT_Reader_LAS();
}

void CT_Reader_LAS::protectedInit()
{
    addNewReadableFormat(FileFormat("las", tr("Fichiers LAS .las")));
}

void CT_Reader_LAS::protectedCreateOutItemDrawableModelList()
{
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_sceneOut, new CT_Scene(), tr("Scène")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_intensityOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Intensité")));
}

bool CT_Reader_LAS::protectedReadFile()
{
    size_t nPoints = m_header->getPointsRecordCount();

    if(nPoints == 0)
    {
        setNotNeedToUseCoordinateSystem();
        return true;
    }

    bool ok = false;

    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            f.seek(m_header->m_offsetToPointData);

            QDataStream stream(&f);
            stream.setByteOrder(QDataStream::LittleEndian);

            qint32 x, y, z;
            double xc, yc, zc;

            quint64 pos = f.pos();
            bool mustTransformPoint = m_header->mustTransformPoints();
            QVector3D center((m_header->m_maxX+m_header->m_minX)/2.0,
                             (m_header->m_maxY+m_header->m_minY)/2.0,
                             (m_header->m_maxZ+m_header->m_minZ)/2.0);

            CT_Repository::CT_AbstractNotModifiablePCIR pcir = PS_REPOSITORY->createNewPointCloud(nPoints);
            CT_StandardCloudStdVectorT<quint16> *intensities = new CT_StandardCloudStdVectorT<quint16>(nPoints);
            /*CT_StandardCloudStdVectorT<quint8> *classifications = new CT_StandardCloudStdVectorT<quint8>(nPoints);
            CT_StandardCloudStdVectorT<quint8> *returns = new CT_StandardCloudStdVectorT<quint8>(nPoints);*/


            for(size_t i=0; i<nPoints; ++i) {
                stream >> x >> y >> z >> intensities->tAt(i);

                CT_Point &p = pcir->tAt(i);

                if(mustTransformPoint)
                {
                    m_header->transformPoint(x, y, z, xc, yc, zc);

                    if(m_centerCloud)
                    {
                        xc -= center.x();
                        yc -= center.y();
                        zc -= center.z();
                    }

                    PS_COORDINATES_SYS->convertImport(xc, yc, zc, p.x, p.y, p.z);
                }
                else
                {
                    if(m_centerCloud)
                        PS_COORDINATES_SYS->convertImport(((double)x)-center.x(), ((double)y)-center.y(), ((double)z)-center.z(), p.x, p.y, p.z);
                    else
                        PS_COORDINATES_SYS->convertImport(x, y, z, p.x, p.y, p.z);
                }

                pos += m_header->m_pointDataRecordLength;
                f.seek(pos);

                setProgress((i*100)/nPoints);
            }

            CT_AbstractCoordinateSystem::realIm xmin, ymin, zmin, xmax, ymax, zmax;

            PS_COORDINATES_SYS->convertImport(m_centerCloud ? m_header->m_minX-center.x() : m_header->m_minX,
                                              m_centerCloud ? m_header->m_minY-center.y() : m_header->m_minY,
                                              m_centerCloud ? m_header->m_minZ-center.z() : m_header->m_minZ,
                                              xmin,
                                              ymin,
                                              zmin);

            PS_COORDINATES_SYS->convertImport(m_centerCloud ? m_header->m_maxX-center.x() : m_header->m_maxX,
                                              m_centerCloud ? m_header->m_maxY-center.y() : m_header->m_maxY,
                                              m_centerCloud ? m_header->m_maxZ-center.z() : m_header->m_maxZ,
                                              xmax,
                                              ymax,
                                              zmax);

            CT_Scene *scene = new CT_Scene(NULL, NULL, pcir);
            scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

            // add the scene
            addOutItemDrawable(DEF_CT_Reader_LAS_sceneOut, scene);

            CT_PointsAttributesScalarTemplated<quint16> *pas = new CT_PointsAttributesScalarTemplated<quint16>(NULL,
                                                                                                               NULL,
                                                                                                               pcir,
                                                                                                               intensities);

            // add intensities
            addOutItemDrawable(DEF_CT_Reader_LAS_intensityOut, pas);
            ok = true;
        }

        f.close();
    }

    return ok;
}

CT_Reader_LAS_Header* CT_Reader_LAS::readHeader(QDataStream &stream, QString &error) const
{
    error = "";
    CT_Reader_LAS_Header header;

    char        buf[40];

    stream.device()->seek(0);
    stream.setByteOrder(QDataStream::LittleEndian);

    // used in #define (readRawData and readData)
    size_t filePos = 0;

    // File Signature
    readRawData(buf, 4, tr("File Signature invalid"));
    buf[4] = '\0';
    header.m_fileSignature = QString(buf);

    if(header.m_fileSignature != "LASF") {
        error = tr("Not a LAS File");
        return NULL;
    }

    // File Source ID
    readData(header.m_fileSourceID, tr("File Source ID invalid"));

    // Global Encoding
    readData(header.m_globalEncoding, tr("Global Encoding invalid"));

    // Project ID
    readRawData(buf, 16, tr("Project ID invalid"));

    for(int i=0; i<16; ++i)
        header.m_projectID[i] = buf[i];

    // Version Major
    readData(header.m_versionMajor, tr("Version Major invalid"));

    // Version Minor
    readData(header.m_versionMinor, tr("Version Minor invalid"));

    // System Identifier
    readRawData(buf, 32, tr("System Identifier invalid"));
    buf[32] = '\0';
    header.m_systemID = QString(buf);

    // Sofware Identifier
    readRawData(buf, 32, tr("Sofware Identifier invalid"));
    buf[32] = '\0';
    header.m_sofwareID = QString(buf);

    // File Creation Day of Year
    readData(header.m_fileCreationDayOfYear, tr("File Creation Day of Year invalid"));

    // File Creation Year
    readData(header.m_fileCreationYear, tr("File Creation Year invalid"));

    // Header Size
    readData(header.m_headerSize, tr("Header Size invalid"));

    // Offset To Point Data
    readData(header.m_offsetToPointData, tr("Offset To Point Data invalid"));

    if(header.m_offsetToPointData < header.m_headerSize) {

        error = tr("The offset to the start of points data (%1) is smaller than the header size (%2).").arg(header.m_offsetToPointData).arg(header.m_headerSize);
        return NULL;
    }

    // Number of Variable Length Records
    readData(header.m_numberOfVariableLengthRecords, tr("Number of Variable Length Records invalid"));

    // Point Data Record Format
    readData(header.m_pointDataRecordFormat, tr("Point Data Record Format invalid"));

    // Point Data Record Length
    readData(header.m_pointDataRecordLength, tr("Point Data Record Length invalid"));

    // Legacy Number of point records
    readData(header.m_legacyNumberOfPointRecord, tr("Legacy Number of point records invalid"));

    // Legacy Number of points by return
    for(int i=0; i<5; ++i) {
        quint32 buf32;
        readData(buf32, tr("Legacy Number of points by return (%1) invalid").arg(i));
        header.m_legacyNumberOfPointsByReturn[i] = buf32;
    }

    readData(header.m_xScaleFactor, tr("X Scale Factor invalid"));
    readData(header.m_yScaleFactor, tr("Y Scale Factor invalid"));
    readData(header.m_zScaleFactor, tr("Z Scale Factor invalid"));
    readData(header.m_xOffset, tr("X Offset invalid"));
    readData(header.m_yOffset, tr("Y Offset invalid"));
    readData(header.m_zOffset, tr("Z Offset invalid"));
    readData(header.m_maxX, tr("Max X invalid"));
    readData(header.m_minX, tr("Min X invalid"));
    readData(header.m_maxY, tr("Max Y invalid"));
    readData(header.m_minY, tr("Min Y invalid"));
    readData(header.m_maxZ, tr("Max Z invalid"));
    readData(header.m_minZ, tr("Min Z invalid"));
    readDataAndCheckSize(header.m_startOfWaveformDataPacketRecord, tr("Start of Waveform Data Packet Record invalid"));
    readDataAndCheckSize(header.m_startOfFirstExtendedVariableLengthRecord, tr("Start Of First Extended Variable Length Record invalid"));
    readDataAndCheckSize(header.m_numberOfExtendedVariableLengthRecords, tr("Number Of Extended Variable Length Records invalid"));
    readDataAndCheckSize(header.m_numberOfPointRecords, tr("Number Of Point Records invalid"));

    for(int i=0; i<15; ++i) {
        quint64 buf64;
        readDataAndCheckSize(buf64, tr("Number Of Points By Return (%1) invalid").arg(i));
        header.m_numberOfPointsByReturn[i] = buf64;
    }

    return new CT_Reader_LAS_Header(header);
}


CT_Reader_LAS_Header::CT_Reader_LAS_Header()
{
    m_fileSourceID = 0;
    m_globalEncoding = 0;

    for(int i=0; i<16; ++i)
        m_projectID[i] = 0;

    m_versionMajor = 0;
    m_versionMinor = 0;
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

    for(int i=0; i<5; ++i)
        m_numberOfPointsByReturn[i] = 0;
}

size_t CT_Reader_LAS_Header::getPointsRecordCount() const
{
    if(m_legacyNumberOfPointRecord == 0)
        return m_numberOfPointRecords;

    return m_legacyNumberOfPointRecord;
}

bool CT_Reader_LAS_Header::mustTransformPoints() const
{
    return (m_xOffset != 0) || (m_yOffset != 0) || (m_zOffset != 0) || (m_xScaleFactor != 1) || (m_yScaleFactor != 1) || (m_zScaleFactor != 1);
}

void CT_Reader_LAS_Header::transformPoint(const qint32 &x, const qint32 &y, const qint32 &z, double &xc, double &yc, double &zc) const
{
    xc = (((double)x)*m_xScaleFactor) + m_xOffset;
    yc = (((double)y)*m_yScaleFactor) + m_yOffset;
    zc = (((double)z)*m_zScaleFactor) + m_zOffset;
}
