#include "ct_reader_las.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_iterator/ct_mutablepointiterator.h"

#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"

#include <QMessageBox>

#include <QFile>
#include <QDataStream>
#include <QDate>

CT_Reader_LAS::CT_Reader_LAS() : CT_AbstractReader()
{    
    m_headerFromConfiguration = NULL;
}

CT_Reader_LAS::CT_Reader_LAS(const CT_Reader_LAS &other) : CT_AbstractReader(other)
{
    m_headerFromConfiguration = NULL;

    if(other.m_headerFromConfiguration != NULL)
        m_headerFromConfiguration = (CT_LASHeader*)other.m_headerFromConfiguration->copy(NULL, NULL, CT_ResultCopyModeList());
}

CT_Reader_LAS::~CT_Reader_LAS()
{
    delete m_headerFromConfiguration;
}

QString CT_Reader_LAS::GetReaderName() const
{
    return tr("Points, format LAS");
}

CT_StepsMenu::LevelPredefined CT_Reader_LAS::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_LAS::setFilePath(const QString &filepath)
{
    QString err;
    CT_LASHeader *header = (CT_LASHeader*)protectedReadHeader(filepath, err);

    if(header != NULL) {
        if(CT_AbstractReader::setFilePath(filepath)) {
            setToolTip(header->toString());
            delete header;
            return true;
        }
    } else if(!err.isEmpty()) {
        PS_LOG->addErrorMessage(LogInterface::reader, err);
    }

    delete header;
    return false;
}

bool CT_Reader_LAS::configure()
{
    QString err;
    CT_LASHeader *header = (CT_LASHeader*)protectedReadHeader(filepath(), err);

    if(header != NULL) {
        delete m_headerFromConfiguration;
        m_headerFromConfiguration = header;
        return true;
    }

    delete header;
    return false;
}

CT_FileHeader* CT_Reader_LAS::createHeaderPrototype() const
{
    return new CT_LASHeader(NULL, NULL);
}

bool CT_Reader_LAS::setAllSettings(const SettingsNodeGroup *settings)
{
    if(CT_AbstractReader::setAllSettings(settings))
    {
        QString error;

        CT_LASHeader *header = (CT_LASHeader*)protectedReadHeader(filepath(), error);

        if(header == NULL)
            return false;

        m_headerFromConfiguration = header;

        return true;
    }

    return false;
}

CT_AbstractReader* CT_Reader_LAS::copy() const
{
    return new CT_Reader_LAS();
}

void CT_Reader_LAS::protectedInit()
{
    addNewReadableFormat(FileFormat("las", tr("Fichiers LAS .las")));

    setToolTip(tr("Charge des points depuis un fichier au format LAS (ASPRS)<br>http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html"));
}

void CT_Reader_LAS::protectedCreateOutItemDrawableModelList()
{
    addOutItemDrawableModel(DEF_CT_Reader_LAS_sceneOut, new CT_Scene(), tr("Scène"));
    addOutItemDrawableModel(DEF_CT_Reader_LAS_attributesOut, new CT_StdLASPointsAttributesContainer(), tr("All Attributs"));

    // CORE of file format from 0 to 5
    if(m_headerFromConfiguration->m_pointDataRecordFormat < 6)
    {
        addOutItemDrawableModel(DEF_CT_Reader_LAS_returnNumberOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Return Number"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_numberOfReturnsOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Number of Returns"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Scan Direction Flag"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut, new CT_PointsAttributesScalarMaskT<PointCore0_5>(), tr("Edge of Flight Line"));
    }
    else // CORE of file format from 6 to 10
    {
        addOutItemDrawableModel(DEF_CT_Reader_LAS_returnNumberOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Return Number"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_numberOfReturnsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Number of Returns"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_classificationFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Classification Flags"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_scannerChannelOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Scanner Channel"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Scan Direction Flag"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut, new CT_PointsAttributesScalarMaskT<PointCore6_10>(), tr("Edge of Flight Line"));
    }

    // CORE of all files
    addOutItemDrawableModel(DEF_CT_Reader_LAS_intensityOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Intensité"));
    addOutItemDrawableModel(DEF_CT_Reader_LAS_classificationOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("Classification"));
    addOutItemDrawableModel(DEF_CT_Reader_LAS_userDataOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("User Data"));
    addOutItemDrawableModel(DEF_CT_Reader_LAS_pointSourceIDOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Point Source ID"));

    // Other options (depend on file format)

    if(m_headerFromConfiguration->m_pointDataRecordFormat < 6)
        addOutItemDrawableModel(DEF_CT_Reader_LAS_scanAngleRankOut, new CT_PointsAttributesScalarTemplated<qint8>(), tr("Scan Angle Rank"));
    else
        addOutItemDrawableModel(DEF_CT_Reader_LAS_scanAngleRankOut, new CT_PointsAttributesScalarTemplated<qint16>(), tr("Scan Angle"));

    // GPS Time
    if((m_headerFromConfiguration->m_pointDataRecordFormat == 1)
            || (m_headerFromConfiguration->m_pointDataRecordFormat > 2))
        addOutItemDrawableModel(DEF_CT_Reader_LAS_gpsTimeOut, new CT_PointsAttributesScalarTemplated<double>(), tr("GPS Time"));

    // Color
    if((m_headerFromConfiguration->m_pointDataRecordFormat == 2)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 3)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 7)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 8)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
    {
        addOutItemDrawableModel(DEF_CT_Reader_LAS_colorOut, new CT_PointsAttributesColor(), tr("Color"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_colorROut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Red"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_colorGOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Green"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_colorBOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("Blue"));
    }

    // Wave Packets
    if((m_headerFromConfiguration->m_pointDataRecordFormat == 4)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 9)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
    {
        addOutItemDrawableModel(DEF_CT_Reader_LAS_wavePacketOut, new CT_PointsAttributesScalarTemplated<quint8>(), tr("Wave Packet Descriptor Index"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_byteOffsetWaveformOut, new CT_PointsAttributesScalarTemplated<quint64>(), tr("Byte offset to waveform data"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_waveformPacketSizeOut, new CT_PointsAttributesScalarTemplated<quint32>(), tr("Waveform packet size in bytes"));
        addOutItemDrawableModel(DEF_CT_Reader_LAS_returnPointWaveformOut, new CT_PointsAttributesScalarTemplated<float>(), tr("Return Point Waveform Location"));
    }

    // NIR
    if((m_headerFromConfiguration->m_pointDataRecordFormat == 8)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
        addOutItemDrawableModel(DEF_CT_Reader_LAS_nirOut, new CT_PointsAttributesScalarTemplated<quint16>(), tr("NIR"));
}

bool CT_Reader_LAS::protectedReadFile()
{
    bool ok = false;
    QString error;

    CT_LASHeader *header = (CT_LASHeader*)protectedReadHeader(filepath(), error);

    if(header == NULL) {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire l'en-tête du fichier %1").arg(filepath()));
        return false;
    }

    if(header->m_pointDataRecordFormat != m_headerFromConfiguration->m_pointDataRecordFormat) {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Le format (%1) du fichier %2 ne correspond pas au format (%3) du fichier lu lors de la configuration").arg(header->m_pointDataRecordFormat)
                                .arg(filepath())
                                .arg(m_headerFromConfiguration->m_pointDataRecordFormat));
        delete header;
        return false;
    }

    size_t nPoints = header->getPointsRecordCount();

    if(nPoints == 0) {
        PS_LOG->addWarningMessage(LogInterface::reader, tr("Aucun points contenu dans le fichier %1").arg(filepath()));
        delete header;
        return true;
    }

    QFile f(filepath());

    if(f.open(QIODevice::ReadOnly))
    {
        setToolTip(header->toString());

        f.seek(header->m_offsetToPointData);

        QDataStream stream(&f);
        stream.setByteOrder(QDataStream::LittleEndian);

        qint32 x, y, z;
        double xc, yc, zc;
        CT_Point pAdded;

        quint64 pos = f.pos();
        bool mustTransformPoint = header->mustTransformPoints();

        CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(nPoints);

        // VECTOR
        CT_StandardCloudStdVectorT<qint8> *scanAngleRankV0_5 = NULL;
        CT_StandardCloudStdVectorT<PointCore0_5> *mask0_5V = NULL;
        CT_StandardCloudStdVectorT<qint16> *scanAngleRankV6_10 = NULL;
        CT_StandardCloudStdVectorT<PointCore6_10> *mask6_10V = NULL;

        if(header->m_pointDataRecordFormat < 6)
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
            colorsV = new CT_ColorCloudStdVector(nPoints);
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

        CT_MutablePointIterator it(pcir);

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
            {
                stream >> scanAngleRankV0_5->tAt(i);
                stream >> userDataV->tAt(i);
            }
            else
            {
                // "user data" is before "scan angle" in version 6 to 10
                stream >> userDataV->tAt(i);
                stream >> scanAngleRankV6_10->tAt(i);
            }

            stream >> pointSourceIDV->tAt(i);

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

                colRGBA.r() = colR/256;
                colRGBA.g() = colG/256;
                colRGBA.b() = colB/256;
                colRGBA.a() = 255;
            }

            // NIR is always after colors if exist
            if(nirV != NULL)
                stream >> nirV->tAt(i);

            // wave packet is always after NIR if exist
            if(wpdiV != NULL)
                stream >> wpdiV->tAt(i) >> botwdV->tAt(i) >> wpsibV->tAt(i) >> rpwlV->tAt(i);

            // CONVERT POINT

            if(mustTransformPoint)
                header->transformPoint(x, y, z, xc, yc, zc);

            pAdded(0) = xc;
            pAdded(1) = yc;
            pAdded(2) = zc;

            it.next().replaceCurrentPoint(pAdded);

            pos += header->m_pointDataRecordLength;
            f.seek(pos);

            setProgress((100.0*i)/nPoints);
        }

        CT_Scene *scene = new CT_Scene(NULL, NULL, pcir);
        scene->setBoundingBox(header->get_minX(), header->get_minY(), header->get_minZ(),
                              header->get_maxX(), header->get_maxY(), header->get_maxZ());

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

        if(mask6_10V == NULL) {
            addOutItemDrawable(DEF_CT_Reader_LAS_returnNumberOut, rn0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_numberOfReturnsOut, nor0_5);
        } else {
            addOutItemDrawable(DEF_CT_Reader_LAS_returnNumberOut, rn6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_numberOfReturnsOut, nor6_10);
        }

        addOutItemDrawable(DEF_CT_Reader_LAS_classificationFlagsOut, cf);
        addOutItemDrawable(DEF_CT_Reader_LAS_scannerChannelOut, sc);

        if(mask6_10V == NULL) {
            addOutItemDrawable(DEF_CT_Reader_LAS_scanDirectionFlagsOut, sdf0_5);
            addOutItemDrawable(DEF_CT_Reader_LAS_edgeOfFlightLineOut, efl0_5);
        } else {
            addOutItemDrawable(DEF_CT_Reader_LAS_scanDirectionFlagsOut, sdf6_10);
            addOutItemDrawable(DEF_CT_Reader_LAS_edgeOfFlightLineOut, efl6_10);
        }

        addOutItemDrawable(DEF_CT_Reader_LAS_classificationOut, new CT_PointsAttributesScalarTemplated<quint8>(NULL, NULL, pcir, classificationsV));

        if(mask6_10V == NULL) {
            addOutItemDrawable(DEF_CT_Reader_LAS_scanAngleRankOut, scanAngleRank0_5);
        } else {
            addOutItemDrawable(DEF_CT_Reader_LAS_scanAngleRankOut, scanAngleRank6_10);
        }

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

        container->insertPointsAttributesAt(CT_LasDefine::Intensity, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_intensityOut));
        container->insertPointsAttributesAt(CT_LasDefine::Return_Number, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_returnNumberOut));
        container->insertPointsAttributesAt(CT_LasDefine::Number_of_Returns, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_numberOfReturnsOut));
        container->insertPointsAttributesAt(CT_LasDefine::Classification_Flags, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_classificationFlagsOut));
        container->insertPointsAttributesAt(CT_LasDefine::Scanner_Channel, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scannerChannelOut));
        container->insertPointsAttributesAt(CT_LasDefine::Scan_Direction_Flag, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scanDirectionFlagsOut));
        container->insertPointsAttributesAt(CT_LasDefine::Edge_of_Flight_Line, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_edgeOfFlightLineOut));
        container->insertPointsAttributesAt(CT_LasDefine::Classification, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_classificationOut));
        container->insertPointsAttributesAt(CT_LasDefine::Scan_Angle_Rank, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_scanAngleRankOut));
        container->insertPointsAttributesAt(CT_LasDefine::User_Data, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_userDataOut));
        container->insertPointsAttributesAt(CT_LasDefine::Point_Source_ID, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_pointSourceIDOut));
        container->insertPointsAttributesAt(CT_LasDefine::GPS_Time, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_gpsTimeOut));
        container->insertPointsAttributesAt(CT_LasDefine::Red, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_colorROut));
        container->insertPointsAttributesAt(CT_LasDefine::Green, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_colorGOut));
        container->insertPointsAttributesAt(CT_LasDefine::Blue, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_colorBOut));
        container->insertPointsAttributesAt(CT_LasDefine::NIR, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_nirOut));
        container->insertPointsAttributesAt(CT_LasDefine::Wave_Packet_Descriptor_Index, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_wavePacketOut));
        container->insertPointsAttributesAt(CT_LasDefine::Byte_offset_to_waveform_data, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_byteOffsetWaveformOut));
        container->insertPointsAttributesAt(CT_LasDefine::Waveform_packet_size_in_bytes, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_waveformPacketSizeOut));
        container->insertPointsAttributesAt(CT_LasDefine::Return_Point_Waveform_Location, (CT_AbstractPointAttributesScalar*)firstItemDrawableOfModel(DEF_CT_Reader_LAS_returnPointWaveformOut));

        ok = true;
    }

    f.close();

    delete header;

    return ok;
}

CT_FileHeader *CT_Reader_LAS::protectedReadHeader(const QString &filepath, QString &error) const
{
    CT_LASHeader *header = NULL;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);

            header = new CT_LASHeader();
            header->setFile(filepath);

            if(!header->read(stream, error)) {
                delete header;
                header = NULL;
            }
        }

        f.close();
    }

    return header;
}
