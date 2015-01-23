#include "pb_lasexporter.h"

#include "ct_reader/ct_reader_las.h"
#include "ct_reader/tools/las/ct_laspointformat0.h"
#include "ct_reader/tools/las/ct_laspointformat1.h"
#include "ct_reader/tools/las/ct_laspointformat2.h"
#include "ct_reader/tools/las/ct_laspointformat3.h"
#include "ct_reader/tools/las/ct_laspointformat4.h"
#include "ct_reader/tools/las/ct_laspointformat5.h"
#include "ct_reader/tools/las/ct_laspointformat6.h"
#include "ct_reader/tools/las/ct_laspointformat7.h"
#include "ct_reader/tools/las/ct_laspointformat8.h"
#include "ct_reader/tools/las/ct_laspointformat9.h"
#include "ct_reader/tools/las/ct_laspointformat10.h"

#include <QFileInfo>
#include <QDateTime>
#include <QInputDialog>
#include <limits>
#include <string.h>

PB_LASExporter::PB_LASExporter() : CT_AbstractExporterAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(true);
    setCanExportWithColors(false);
    setCanExportWithNormals(false);

    m_lasContainer = NULL;
}

QString PB_LASExporter::getExporterCustomName() const
{
    return "Points, LAS";
}

void PB_LASExporter::init()
{
    addNewExportFormat(FileFormat("las", tr("Fichiers LAS .las")));
}

bool PB_LASExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_IAccessPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun ItemDrawable du type CT_IAccessPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_LASExporter::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
{
    QList<CT_AbstractCloudIndex*> myList;
    QListIterator<CT_AbstractCloudIndex*> it(list);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *item = it.next();

        if(dynamic_cast<CT_AbstractCloudIndexT<CT_Point>*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun points à exporter"));
        return false;
    }

    return CT_AbstractExporter::setPointsToExport(myList);
}

CT_ItemDrawableHierarchyCollectionWidget::CloudType PB_LASExporter::cloudType() const
{
    return CT_ItemDrawableHierarchyCollectionWidget::POINTS;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > PB_LASExporter::getBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l;
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Attributs"), (CT_AbstractItemDrawableCollectionBuilder*)&m_lasAttributesContainerBuilder);

    return l;
}

void PB_LASExporter::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    // attributes
    if((&m_lasAttributesContainerBuilder) == pair.second)
        model->addExcludeModel(0); // exclude me
}

bool PB_LASExporter::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
    QListIterator<CT_AbstractSingularItemDrawable*> it(list);

    m_lasContainer = NULL;

    while(it.hasNext() && (m_lasContainer == NULL))
        m_lasContainer = dynamic_cast<CT_StdLASPointsAttributesContainer*>(it.next());

    return true;
}

CT_AbstractExporter* PB_LASExporter::copy() const
{
    return new PB_LASExporter();
}

bool PB_LASExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "las";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly))
    {
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        bool ok = false;

        CT_AbstractLASPointFormat *toolsFormat = createPointDataFormat();

        if(toolsFormat != NULL)
        {
            ok = true;

            QList<CT_AbstractCloudIndexT<CT_Point>*> indexes;

            QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

            while(it.hasNext())
            {
                CT_AbstractItemDrawable *item = it.next();

                const CT_AbstractCloudIndexT<CT_Point> *index = dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex();

                if(index != NULL)
                    indexes.append((CT_AbstractCloudIndexT<CT_Point>*)index);
            }

            QListIterator<CT_AbstractCloudIndex*> itP(pointsToExport());

            while(itP.hasNext())
                indexes.append(dynamic_cast<CT_AbstractCloudIndexT<CT_Point>*>(itP.next()));

            size_t nPoints = 0;

            // write header
            CT_LASHeader *header = writeHeader(stream, (m_lasContainer == NULL ? NULL : (CT_AbstractPointAttributesScalar*)m_lasContainer->pointsAttributesAt(CT_LasDefine::Return_Number)), indexes, nPoints);

            if(header == NULL)
            {
                setErrorMessage(tr("Problème lors de l'écriture de l'en-tête du fichier"));
                ok = false;
            }
            else
            {
                // write points
                toolsFormat->setHeader(header);

                if(m_lasContainer != NULL)
                    toolsFormat->setAttributes(m_lasContainer->lasPointsAttributes());

                toolsFormat->initWrite();

                size_t pCount = 0;

                QListIterator<CT_AbstractCloudIndexT<CT_Point>*> itIL(indexes);

                while(itIL.hasNext())
                {
                    CT_AbstractCloudIndexT<CT_Point> *index = itIL.next();

                    CT_AbstractCloudIndexT<CT_Point>::ConstIterator itI = index->constBegin();
                    CT_AbstractCloudIndexT<CT_Point>::ConstIterator end = index->constEnd();

                    while(itI != end) {
                        toolsFormat->write(stream, itI.cIndex());

                        ++itI;
                        ++pCount;

                        setExportProgress((pCount*100)/nPoints);
                    }
                }

                if(nPoints == 0)
                    setNotNeedToUseCoordinateSystem();
            }

            delete header;
        }

        clearWorker();

        file.close();
        return ok;
    }

    clearWorker();

    return false;
}

void PB_LASExporter::clearWorker()
{
    m_lasContainer = NULL;
}

CT_LASHeader* PB_LASExporter::writeHeader(QDataStream &stream,
                                          const CT_AbstractPointAttributesScalar *rn,
                                          const QList<CT_AbstractCloudIndexT<CT_Point> *> &indexes,
                                          size_t &nPoints)
{
    CT_LASHeader *header = NULL;
    nPoints = 0;

    int format = getPointDataFormat();

    if(format >= 0 && format < 11)
    {
        header = new CT_LASHeader();

        QVector3D min;
        min.setX(std::numeric_limits<double>::max());
        min.setY(min.x());
        min.setZ(min.x());

        QVector3D max;
        max.setX(-min.x());
        max.setY(max.x());
        max.setZ(max.x());

        for(int i=0; i<15; ++i)
            header->m_numberOfPointsByReturn[i] = 0;

        const CT_AbstractCloudIndexT<CT_Point> *rnIndex = NULL;
        size_t rnSize = 0;

        if(rn != NULL)
        {
            rnIndex = rn->getPointCloudIndex();

            if(rnIndex != NULL)
                rnSize = rnIndex->size();
        }

        QListIterator<CT_AbstractCloudIndexT<CT_Point>*> it(indexes);

        while(it.hasNext())
        {
            CT_AbstractCloudIndexT<CT_Point> *index = it.next();

            CT_AbstractCloudIndexT<CT_Point>::ConstIterator itP = index->constBegin();
            CT_AbstractCloudIndexT<CT_Point>::ConstIterator end = index->constEnd();

            // count the number of points
            nPoints += index->size();

            // compute max, min and return number
            while(itP != end) {
                const CT_Point &p = itP.cT();

                if(p(0) > max.x())
                    max.setX(p(0));

                if(p(1) > max.y())
                    max.setY(p(1));

                if(p(2) > max.z())
                    max.setZ(p(2));

                if(p(0) < min.x())
                    min.setX(p(0));

                if(p(1) < min.y())
                    min.setY(p(1));

                if(p(2) < min.z())
                    min.setZ(p(2));

                if(rnIndex != NULL) {
                    size_t i = rnIndex->indexOf(itP.cIndex());

                    if(i < rnSize)
                    {
                        quint64 &val = header->m_numberOfPointsByReturn[(int)rn->dValueAt(i)-1];
                        ++val;
                    }
                }

                ++itP;
            }
        }

        if(rn == NULL)
            header->m_numberOfPointsByReturn[0] = nPoints;

        QString error;
        QDate date = QDate::currentDate();

        header->m_fileSourceID = 1;
        header->m_globalEncoding = 1;
        header->m_projectIDGuidData1 = 0;
        header->m_projectIDGuidData2 = 0;
        header->m_projectIDGuidData3 = 0;
        header->m_projectIDGuidData4 = 0;
        header->m_versionMajor = 1;  // format 1.4
        header->m_versionMinor = 4;
        #ifdef _MSC_VER
        strcpy_s(header->m_systemID, "EXPORT");
        strcpy_s(header->m_sofwareID, "Computree 3.0");
        #else
        strcpy(header->m_systemID, "EXPORT");
        strcpy(header->m_sofwareID, "Computree 3.0");
        #endif
        header->m_fileCreationDayOfYear = date.dayOfYear();
        header->m_fileCreationYear = date.year();
        header->m_headerSize = 0; // Automatically compute in "write" of the header
        header->m_offsetToPointData = 0; // Automatically compute in "write" of the header
        header->m_numberOfVariableLengthRecords = 0; // TODO : write the good value
        header->m_pointDataRecordFormat = format;
        header->m_pointDataRecordLength = getPointDataLength(format);
        header->m_legacyNumberOfPointRecord = 0; // deprecated in 1.4

        for(int i=0; i<5; ++i)
            header->m_legacyNumberOfPointsByReturn[i] = 0; // deprecated in 1.4

        CT_AbstractCoordinateSystem::realEx max_xc, max_yc, max_zc;
        CT_AbstractCoordinateSystem::realEx min_xc, min_yc, min_zc;

        PS_COORDINATES_SYS->convertExport(max.x(), max.y(), max.z(), max_xc, max_yc, max_zc);
        PS_COORDINATES_SYS->convertExport(min.x(), min.y(), min.z(), min_xc, min_yc, min_zc);

        // maximum point coordinate
        double scaleFactor = qMax(qMax(qAbs(max_xc), qAbs(max_yc)), qAbs(max_zc));
        scaleFactor = qMax(qMax(qMax(qAbs(min_xc), qAbs(min_yc)), qAbs(min_zc)), (CT_AbstractCoordinateSystem::realEx)scaleFactor);

        // scale factor is the optimized to convert a point to be the most accurate
        scaleFactor = qMin((scaleFactor/(std::numeric_limits<qint32>::max()-1)), 1.0);

        header->m_xScaleFactor = scaleFactor;
        header->m_yScaleFactor = scaleFactor;
        header->m_zScaleFactor = scaleFactor;
        header->m_xOffset = 0;
        header->m_yOffset = 0;
        header->m_zOffset = 0;
        header->setMinAndMaxCoordinates(min_xc, min_yc, min_zc, max_xc, max_yc, max_zc);
        header->m_startOfWaveformDataPacketRecord = 0; // TODO : write the good value
        header->m_startOfFirstExtendedVariableLengthRecord = 0; // TODO : write the good value
        header->m_numberOfExtendedVariableLengthRecords = 0; // TODO : write the good value
        header->m_numberOfPointRecords = nPoints;

        if(!header->write(stream, error))
        {
            setErrorMessage(error);
            delete header;
            header = NULL;
        }
        else
            return header;
    }

    return header;
}

int PB_LASExporter::getPointDataFormat() const
{
    if(m_lasContainer == NULL)
        return 6;

    QList<CT_LasDefine::LASPointAttributesType> list = m_lasContainer->lasPointAttributesTypes();

    bool wavePacket = true;
    bool nir = true;
    bool colors = true;

    if(!list.contains(CT_LasDefine::Wave_Packet_Descriptor_Index)
            && !list.contains(CT_LasDefine::Byte_offset_to_waveform_data)
            && !list.contains(CT_LasDefine::Waveform_packet_size_in_bytes)
            && !list.contains(CT_LasDefine::Return_Point_Waveform_Location))
        wavePacket = false;

    if(!list.contains(CT_LasDefine::NIR))
        nir = false;

    if(!list.contains(CT_LasDefine::Red)
            && !list.contains(CT_LasDefine::Green)
            && !list.contains(CT_LasDefine::Blue))
        colors = false;

    if(wavePacket)
    {
        if(nir || colors)
            return 10;

        return 9;
    }

    if(nir)
        return 8;

    if(colors)
        return 7;

    return 6;
}

size_t PB_LASExporter::getPointDataLength(const int &optFormat) const
{
    int format = optFormat;

    if((format < 0) || (format > 10))
        format = getPointDataFormat();

    if(format == 0)
        return CT_LASPointFormat0().sizeInBytes();
    else if(format == 1)
        return CT_LASPointFormat1().sizeInBytes();
    else if(format == 2)
        return CT_LASPointFormat2().sizeInBytes();
    else if(format == 3)
        return CT_LASPointFormat3().sizeInBytes();
    else if(format == 4)
        return CT_LASPointFormat4().sizeInBytes();
    else if(format == 5)
        return CT_LASPointFormat5().sizeInBytes();
    else if(format == 6)
        return CT_LASPointFormat6().sizeInBytes();
    else if(format == 7)
        return CT_LASPointFormat7().sizeInBytes();
    else if(format == 8)
        return CT_LASPointFormat8().sizeInBytes();
    else if(format == 9)
        return CT_LASPointFormat9().sizeInBytes();
    else if(format == 10)
        return CT_LASPointFormat10().sizeInBytes();

    return 0;
}

CT_AbstractLASPointFormat *PB_LASExporter::createPointDataFormat(const int &optFormat) const
{
    int format = optFormat;

    if((format < 0) || (format > 10))
        format = getPointDataFormat();

    if(format == 0)
        return new CT_LASPointFormat0();
    else if(format == 1)
        return new CT_LASPointFormat1();
    else if(format == 2)
        return new CT_LASPointFormat2();
    else if(format == 3)
        return new CT_LASPointFormat3();
    else if(format == 4)
        return new CT_LASPointFormat4();
    else if(format == 5)
        return new CT_LASPointFormat5();
    else if(format == 6)
        return new CT_LASPointFormat6();
    else if(format == 7)
        return new CT_LASPointFormat7();
    else if(format == 8)
        return new CT_LASPointFormat8();
    else if(format == 9)
        return new CT_LASPointFormat9();
    else if(format == 10)
        return new CT_LASPointFormat10();

    return NULL;
}
