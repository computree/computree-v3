#include "ct_reader_las.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_stdlaspointsattributescontainer.h"
#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

#include <QMessageBox>

#include <QFile>
#include <QDataStream>

#define readRawDataAndCheck(Param, N, Err) if(stream.readRawData(Param, N) != N) { error = Err; return NULL; } else { filePos += N; }
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
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_attributesOut, new CT_StdLASPointsAttributesContainer(), tr("All Attributs")));

    // CORE of file format from 0 to 5
    if(m_header->m_pointDataRecordFormat < 6)
    {
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_returnNumberOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Return Number")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_numberOfReturnsOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Number of Returns")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Scan Direction Flag")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Edge of Flight Line")));
    }
    else // CORE of file format from 6 to 10
    {
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_returnNumberOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Return Number")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_numberOfReturnsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Number of Returns")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_classificationFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Classification Flags")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_scannerChannelOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Scanner Channel")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Scan Direction Flag")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Edge of Flight Line")));
    }

    // CORE of all files
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_intensityOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Intensité")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_classificationOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("Classification")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_userDataOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("User Data")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_pointSourceIDOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Point Source ID")));

    // Other options (depend on file format)

    if(m_header->m_pointDataRecordFormat < 6)
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_scanAngleRankOut, new CT_PointsAttributesScalarTemplated<qint8>(), tr("Scan Angle Rank")));
    else
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_scanAngleRankOut, new CT_PointsAttributesScalarTemplated<qint16>(), tr("Scan Angle")));

    // GPS Time
    if((m_header->m_pointDataRecordFormat == 1)
            || (m_header->m_pointDataRecordFormat > 2))
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_gpsTimeOut, new CT_PointsAttributesScalarTemplated<double>(), tr("GPS Time")));

    // Color
    if((m_header->m_pointDataRecordFormat == 2)
            || (m_header->m_pointDataRecordFormat == 3)
            || (m_header->m_pointDataRecordFormat == 5)
            || (m_header->m_pointDataRecordFormat == 7)
            || (m_header->m_pointDataRecordFormat == 8)
            || (m_header->m_pointDataRecordFormat == 10))
    {
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_colorOut, new CT_PointsAttributesColor(), tr("Color")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_colorROut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Red")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_colorGOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Green")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_colorBOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Blue")));
    }

    // Wave Packets
    if((m_header->m_pointDataRecordFormat == 4)
            || (m_header->m_pointDataRecordFormat == 5)
            || (m_header->m_pointDataRecordFormat == 9)
            || (m_header->m_pointDataRecordFormat == 10))
    {
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_wavePacketOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("Wave Packet Descriptor Index")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_byteOffsetWaveformOut, new CT_PointsAttributesScalarTemplated<quint64>(), tr("Byte offset to waveform data")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_waveformPacketSizeOut, new CT_PointsAttributesScalarTemplated<quint32>(), tr("Waveform packet size in bytes")));
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_returnPointWaveformOut, new CT_PointsAttributesScalarTemplated<float>(), tr("Return Point Waveform Location")));
    }

    // NIR
    if((m_header->m_pointDataRecordFormat == 8)
            || (m_header->m_pointDataRecordFormat == 10))
        addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_LAS_nirOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("NIR")));
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

            // VECTOR
            CT_StandardCloudStdVectorT<qint8> *scanAngleRankV0_5 = NULL;
            CT_StandardCloudStdVectorT<PointCore0_5> *mask0_5V = NULL;
            CT_StandardCloudStdVectorT<qint16> *scanAngleRankV6_10 = NULL;
            CT_StandardCloudStdVectorT<PointCore6_10> *mask6_10V = NULL;

            if(m_header->m_pointDataRecordFormat < 6)
            {
                mask0_5V = new CT_StandardCloudStdVectorT<PointCore0_5>(nPoints);
                scanAngleRankV0_5 = new CT_StandardCloudStdVectorT<qint8>(nPoints);
            }
            else
            {
                mask6_10V = new CT_StandardCloudStdVectorT<PointCore6_10>(nPoints);
                scanAngleRankV6_10 = new CT_StandardCloudStdVectorT<qint16>(nPoints);
            }

            // ItemDrawable 0_5
            CT_PointsAttributesScalarMaskT<PointCore0_5> *rn0_5 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore0_5> *nor0_5 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore0_5> *sdf0_5 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore0_5> *efl0_5 = NULL;
            CT_PointsAttributesScalarTemplated<qint8> *scanAngleRank0_5 = NULL;

            // ItemDrawable 6_10
            CT_PointsAttributesScalarMaskT<PointCore6_10> *rn6_10 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore6_10> *nor6_10 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore6_10> *sdf6_10 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore6_10> *efl6_10 =  NULL;
            CT_PointsAttributesScalarTemplated<qint16> *scanAngleRank6_10 = NULL;
            CT_PointsAttributesScalarMaskT<PointCore6_10> *cf = NULL;
            CT_PointsAttributesScalarMaskT<PointCore6_10> *sc = NULL;

            // Vector in all point data format :
            CT_StandardCloudStdVectorT<quint16> *intensitiesV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
            CT_StandardCloudStdVectorT<quint8> *classificationsV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
            CT_StandardCloudStdVectorT<quint8> *userDataV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
            CT_StandardCloudStdVectorT<quint16> *pointSourceIDV = new CT_StandardCloudStdVectorT<quint16>(nPoints);

            // Vector not in all point data format :
            CT_StandardCloudStdVectorT<double> *gpsTimeV = NULL;
            CT_ColorCloudStdVector *colorsV = NULL;
            CT_StandardCloudStdVectorT<quint16> *colorsRV = NULL;
            CT_StandardCloudStdVectorT<quint16> *colorsGV = NULL;
            CT_StandardCloudStdVectorT<quint16> *colorsBV = NULL;
            CT_StandardCloudStdVectorT<quint8> *wpdiV = NULL;
            CT_StandardCloudStdVectorT<quint64> *botwdV = NULL;
            CT_StandardCloudStdVectorT<quint32> *wpsibV = NULL;
            CT_StandardCloudStdVectorT<float> *rpwlV = NULL;
            CT_StandardCloudStdVectorT<quint16> *nirV = NULL;

            if(containsItemDrawableModel(DEF_CT_Reader_LAS_gpsTimeOut))
                gpsTimeV = new CT_StandardCloudStdVectorT<double>(nPoints);

            if(containsItemDrawableModel(DEF_CT_Reader_LAS_colorOut))
            {
                colorsV = new CT_ColorCloudStdVector(nPoints, true);
                colorsRV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
                colorsGV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
                colorsBV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
            }

            if(containsItemDrawableModel(DEF_CT_Reader_LAS_nirOut))
                nirV = new CT_StandardCloudStdVectorT<quint16>(nPoints);

            if(containsItemDrawableModel(DEF_CT_Reader_LAS_wavePacketOut))
            {
                wpdiV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
                botwdV = new CT_StandardCloudStdVectorT<quint64>(nPoints);
                wpsibV = new CT_StandardCloudStdVectorT<quint32>(nPoints);
                rpwlV = new CT_StandardCloudStdVectorT<float>();
            }

            for(size_t i=0; i<nPoints; ++i)
            {
                // READ ALL ATTRIBUTES
                stream >> x >> y >> z >> intensitiesV->tAt(i);

                if(mask6_10V == NULL)
                    stream >> mask0_5V->tAt(i).entire;
                else
                    stream >> mask6_10V->tAt(i).entire;

                stream >> classificationsV->tAt(i);

                if(scanAngleRankV6_10 == NULL)
                    stream >> scanAngleRankV0_5->tAt(i);
                else
                    stream >> scanAngleRankV6_10->tAt(i);

                stream >> userDataV->tAt(i) >> pointSourceIDV->tAt(i);

                // gps time is always after pointSourceID
                if(gpsTimeV != NULL)
                    stream >> gpsTimeV->tAt(i);

                // color is always after gpsTime if exist otherwise after pointSourceID
                if(colorsRV != NULL)
                {
                    CT_Color &colRGBA = colorsV->tAt(i);
                    quint16 &colR = colorsRV->tAt(i);
                    quint16 &colG = colorsGV->tAt(i);
                    quint16 &colB = colorsBV->tAt(i);

                    stream >> colR >> colG >> colB;

                    colRGBA.r = colR/256;
                    colRGBA.g = colG/256;
                    colRGBA.b = colB/256;
                    colRGBA.a = 255;
                }

                // NIR is always after colors if exist
                if(nirV != NULL)
                    stream >> nirV->tAt(i);

                // wave packet is always after NIR if exist
                if(wpdiV != NULL)
                    stream >> wpdiV->tAt(i) >> botwdV->tAt(i) >> wpsibV->tAt(i) >> rpwlV->tAt(i);

                // CONVERT POINT

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

            // add attributes
            CT_StdLASPointsAttributesContainer *container = new CT_StdLASPointsAttributesContainer(NULL,
                                                                                                   NULL);

            if(mask6_10V == NULL)
            {
                // 0b00000111
                rn0_5 = new CT_PointsAttributesScalarMaskT<PointCore0_5>(NULL,NULL,7,0,mask0_5V,pcir,true);

                // 0b00111000
                nor0_5 = new CT_PointsAttributesScalarMaskT<PointCore0_5>(NULL,NULL,56,3,mask0_5V,pcir,false);

                 // 0b01000000
                sdf0_5 = new CT_PointsAttributesScalarMaskT<PointCore0_5>(NULL,NULL,64,6,mask0_5V,pcir,false);

                // 0b10000000
                efl0_5 = new CT_PointsAttributesScalarMaskT<PointCore0_5>(NULL,NULL,128, 7,mask0_5V,pcir,false);

                scanAngleRank0_5 = new CT_PointsAttributesScalarTemplated<qint8>(NULL, NULL, pcir, scanAngleRankV0_5);

            }
            else
            {
                // 0b0000 0000 0000 1111
                rn6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,15,0,mask6_10V,pcir,true);

                // 0b0000 0000 1111 0000
                nor6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,240, 4,mask6_10V,pcir,false);

                // 0b0000 0111 0000 0000
                cf = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,1792, 8,mask6_10V,pcir,false);

                // 0b0001 1000 0000 0000
                sc = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,6144, 11,mask6_10V,pcir,false);

                // 0b0100 0000 0000 0000
                sdf6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,16384, 14,mask6_10V,pcir,false);

                // 0b1000 0000 0000 0000
                efl6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(NULL,NULL,32768, 15,mask6_10V,pcir,false);

                scanAngleRank6_10 = new CT_PointsAttributesScalarTemplated<qint16>(NULL, NULL, pcir, scanAngleRankV6_10);

                /*for(int iii=0;iii<10; ++iii)
                {
                    double tmp1 = nor6_10->dValueAt(iii);
                    double tmp2 = rn6_10->dValueAt(iii);
                    double tmp3 = sdf6_10->dValueAt(iii);
                    double tmp4 = efl6_10->dValueAt(iii);
                    double tmp5 = tmp1;
                }*/
            }

            addOutItemDrawable(DEF_CT_Reader_LAS_attributesOut, container);

            addOutItemDrawable(DEF_CT_Reader_LAS_intensityOut, new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, intensitiesV));
            addOutItemDrawable(DEF_CT_Reader_LAS_returnNumberOut, rn0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_returnNumberOut, rn6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_numberOfReturnsOut, nor0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_numberOfReturnsOut, nor6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_classificationFlagsOut, cf);
            addOutItemDrawable(DEF_CT_Reader_LAS_scannerChannelOut, sc);
            addOutItemDrawable(DEF_CT_Reader_LAS_scanDirectionFlagsOut, sdf0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_scanDirectionFlagsOut, sdf6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_edgeOfFlightLineOut, efl0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_edgeOfFlightLineOut, efl6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_classificationOut, new CT_PointsAttributesScalarTemplated<quint8>(NULL, NULL, pcir, classificationsV));
            addOutItemDrawable(DEF_CT_Reader_LAS_scanAngleRankOut, scanAngleRank0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_scanAngleRankOut, scanAngleRank6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_userDataOut, new CT_PointsAttributesScalarTemplated<quint8>(NULL, NULL, pcir, userDataV));
            addOutItemDrawable(DEF_CT_Reader_LAS_pointSourceIDOut, new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, pointSourceIDV));
            addOutItemDrawable(DEF_CT_Reader_LAS_gpsTimeOut, (gpsTimeV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<double>(NULL, NULL, pcir, gpsTimeV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_colorOut, (colorsV == NULL ? NULL : new CT_PointsAttributesColor(NULL, NULL, pcir, colorsV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_colorROut, (colorsRV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, colorsRV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_colorGOut, (colorsGV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, colorsGV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_colorBOut, (colorsBV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, colorsBV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_wavePacketOut, (wpdiV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint8>(NULL, NULL, pcir, wpdiV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_byteOffsetWaveformOut, (botwdV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint64>(NULL, NULL, pcir, botwdV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_waveformPacketSizeOut, (wpsibV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint32>(NULL, NULL, pcir, wpsibV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_returnPointWaveformOut, (rpwlV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<float>(NULL, NULL, pcir, rpwlV)));
            addOutItemDrawable(DEF_CT_Reader_LAS_nirOut, (nirV == NULL ? NULL : new CT_PointsAttributesScalarTemplated<quint16>(NULL, NULL, pcir, nirV)));

            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Intensity, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_intensityOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Return_Number, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_returnNumberOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Number_of_Returns, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_numberOfReturnsOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Classification_Flags, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_classificationFlagsOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Scanner_Channel, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scannerChannelOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Scan_Direction_Flag, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Edge_of_Flight_Line, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Classification, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_classificationOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Scan_Angle_Rank, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scanAngleRankOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::User_Data, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_userDataOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Point_Source_ID, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_pointSourceIDOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::GPS_Time, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_gpsTimeOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Color, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_colorOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Wave_Packet_Descriptor_Index, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_wavePacketOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Byte_offset_to_waveform_data, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_byteOffsetWaveformOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Waveform_packet_size_in_bytes, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_waveformPacketSizeOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::Return_Point_Waveform_Location, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_returnPointWaveformOut));
            container->insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::NIR, (CT_AbstractPointsAttributes*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_nirOut));

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

    // used in #define (readRawDataAndCheck and readData)
    size_t filePos = 0;

    // File Signature
    readRawDataAndCheck(buf, 4, tr("File Signature invalid"));
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
    readRawDataAndCheck(buf, 16, tr("Project ID invalid"));

    for(int i=0; i<16; ++i)
        header.m_projectID[i] = buf[i];

    // Version Major
    readData(header.m_versionMajor, tr("Version Major invalid"));

    // Version Minor
    readData(header.m_versionMinor, tr("Version Minor invalid"));

    // System Identifier
    readRawDataAndCheck(buf, 32, tr("System Identifier invalid"));
    buf[32] = '\0';
    header.m_systemID = QString(buf);

    // Sofware Identifier
    readRawDataAndCheck(buf, 32, tr("Sofware Identifier invalid"));
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
