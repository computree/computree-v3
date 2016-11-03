#include "ct_exporter_las.h"

#include "ctliblas/tools/las/ct_laspointformat0.h"
#include "ctliblas/tools/las/ct_laspointformat1.h"
#include "ctliblas/tools/las/ct_laspointformat2.h"
#include "ctliblas/tools/las/ct_laspointformat3.h"
#include "ctliblas/tools/las/ct_laspointformat4.h"
#include "ctliblas/tools/las/ct_laspointformat5.h"
#include "ctliblas/tools/las/ct_laspointformat6.h"
#include "ctliblas/tools/las/ct_laspointformat7.h"
#include "ctliblas/tools/las/ct_laspointformat8.h"
#include "ctliblas/tools/las/ct_laspointformat9.h"
#include "ctliblas/tools/las/ct_laspointformat10.h"

#include "ct_iterator/ct_pointiterator.h"

#include "ct_itemdrawable/tools/ct_itemsearchhelper.h"

#include "ct_model/tools/ct_modelsaverestorehelper.h"

#include <QFileInfo>
#include <QDateTime>
#include <QInputDialog>
#include <limits>
#include <string.h>
#include <QDebug>

CT_Exporter_LAS::CT_Exporter_LAS() : CT_AbstractExporterAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(true);
    setCanExportWithColors(false);
    setCanExportWithNormals(false);

    m_lasContainer = NULL;
    m_lasContainerModel = NULL;
}

QString CT_Exporter_LAS::getExporterCustomName() const
{
    return tr("Points, LAS");
}

CT_StepsMenu::LevelPredefined CT_Exporter_LAS::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}


void CT_Exporter_LAS::init()
{
    addNewExportFormat(FileFormat("las", tr("Fichiers LAS .las")));

    setToolTip(tr("Exporte tous les points au format LAS, ASPRS<br>http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html"));
}


bool CT_Exporter_LAS::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
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

bool CT_Exporter_LAS::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
{
    QList<CT_AbstractCloudIndex*> myList;
    QListIterator<CT_AbstractCloudIndex*> it(list);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *item = it.next();

        if(dynamic_cast<CT_AbstractPointCloudIndex*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun points à exporter"));
        return false;
    }

    return CT_AbstractExporter::setPointsToExport(myList);
}

SettingsNodeGroup* CT_Exporter_LAS::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporterAttributesSelection::saveExportConfiguration();

    SettingsNodeGroup *myRoot = new SettingsNodeGroup("CT_Exporter_LAS");

    if(m_lasContainerModel != NULL) {

        CT_ModelSaveRestoreHelper helper;
        myRoot->addGroup(helper.saveToSearchOutModel(m_lasContainerModel, "LasContainerModel"));
    }

    root->addGroup(myRoot);

    return root;
}

bool CT_Exporter_LAS::loadExportConfiguration(const SettingsNodeGroup *root)
{
    clearWorker();

    if(CT_AbstractExporterAttributesSelection::loadExportConfiguration(root))
    {
        QList<SettingsNodeGroup*> groups = root->groupsByTagName("CT_Exporter_LAS");

        if(groups.isEmpty())
            return true;

        groups = groups.first()->groupsByTagName("LasContainerModel");

        if(groups.isEmpty())
            return true;

        CT_ModelSaveRestoreHelper helper;
        m_lasContainerModel = dynamic_cast<CT_OutAbstractSingularItemModel *>(helper.searchModelFromSettings(groups.first(), myStep()));

        return (m_lasContainerModel != NULL);
    }

    return false;
}

CT_ItemDrawableHierarchyCollectionWidget::CloudType CT_Exporter_LAS::cloudType() const
{
    return CT_ItemDrawableHierarchyCollectionWidget::POINTS;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > CT_Exporter_LAS::getBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l;
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Attributs"), (CT_AbstractItemDrawableCollectionBuilder*)&m_lasAttributesContainerBuilder);

    return l;
}

void CT_Exporter_LAS::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    // attributes
    if((&m_lasAttributesContainerBuilder) == pair.second)
        model->addExcludeModel(0); // exclude me
}

bool CT_Exporter_LAS::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearWorker();

    QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
    QListIterator<CT_AbstractSingularItemDrawable*> it(list);

    while(it.hasNext() && (m_lasContainer == NULL))
        m_lasContainer = dynamic_cast<CT_StdLASPointsAttributesContainer*>(it.next());

    // the las container can not be found if this exporter is used in a step
    if(m_lasContainer == NULL) {

        // search the model to use
        QList<CT_OutAbstractSingularItemModel*> listM = selectorWidget->itemDrawableModelSelected();
        QListIterator<CT_OutAbstractSingularItemModel*> itM(listM);

        while(itM.hasNext() && (m_lasContainerModel == NULL)) {
            CT_OutAbstractSingularItemModel *model = itM.next();
            if(dynamic_cast<CT_StdLASPointsAttributesContainer*>(model->itemDrawable()) != NULL)
                m_lasContainerModel = model;
        }
    }

    return true;
}

CT_AbstractExporter* CT_Exporter_LAS::copy() const
{
    return new CT_Exporter_LAS();
}

bool CT_Exporter_LAS::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "las";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    if(m_lasContainerModel != NULL) {

        CT_ItemSearchHelper helper;
        CT_ResultItemIterator it = helper.searchSingularItemsForModel(m_lasContainerModel);

        if(it.hasNext())
            m_lasContainer = dynamic_cast<CT_StdLASPointsAttributesContainer*>((CT_AbstractSingularItemDrawable*)it.next());
    }

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

            QList<CT_AbstractPointCloudIndex*> indexes;

            QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

            while(it.hasNext())
            {
                CT_AbstractItemDrawable *item = it.next();

                const CT_AbstractPointCloudIndex *index = dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex();

                if(index != NULL)
                    indexes.append((CT_AbstractPointCloudIndex*)index);
            }

            QListIterator<CT_AbstractCloudIndex*> itP(pointsToExport());

            while(itP.hasNext())
                indexes.append(dynamic_cast<CT_AbstractPointCloudIndex*>(itP.next()));

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

                QListIterator<CT_AbstractPointCloudIndex*> itIL(indexes);

                while(itIL.hasNext())
                {
                    CT_AbstractPointCloudIndex *index = itIL.next();

                    CT_PointIterator itI(index);

                    while(itI.hasNext()) {
                        itI.next();
                        toolsFormat->write(stream, itI.currentPoint(), itI.cIndex());

                        ++pCount;

                        setExportProgress((pCount*100)/nPoints);
                    }
                }
            }

            delete header;
        }

        file.close();
        return ok;
    }

    return false;
}

void CT_Exporter_LAS::clearWorker()
{
    m_lasContainer = NULL;
    m_lasContainerModel = NULL;
}

CT_LASHeader* CT_Exporter_LAS::writeHeader(QDataStream &stream,
                                          const CT_AbstractPointAttributesScalar *rn,
                                          const QList<CT_AbstractPointCloudIndex *> &indexes,
                                          size_t &nPoints)
{
    CT_LASHeader *header = NULL;
    nPoints = 0;

    int format = getPointDataFormat();


    if(format >= 0 && format < 11)
    {
        header = new CT_LASHeader();

        CT_Point min;
        min.setX(std::numeric_limits<double>::max());
        min.setY(min.x());
        min.setZ(min.x());

        CT_Point max;
        max.setX(-min.x());
        max.setY(max.x());
        max.setZ(max.x());

        for(int i=0; i<5; ++i)
            header->m_legacyNumberOfPointsByReturn[i] = 0; // deprecated in 1.4

        for(int i=0; i<15; ++i)
            header->m_numberOfPointsByReturn[i] = 0;

        const CT_AbstractPointCloudIndex *rnIndex = NULL;
        size_t rnSize = 0;

        if(rn != NULL)
        {
            rnIndex = rn->getPointCloudIndex();

            if(rnIndex != NULL)
                rnSize = rnIndex->size();
        }

        QListIterator<CT_AbstractPointCloudIndex*> it(indexes);

        while(it.hasNext())
        {
            CT_AbstractPointCloudIndex *index = it.next();

            CT_PointIterator itP(index);

            // count the number of points
            nPoints += index->size();

            // compute max, min and return number
            while(itP.hasNext()) {

                const CT_Point &p = itP.next().currentPoint();

                if(p(0) > max(0))
                    max.setX(p(0));

                if(p(1) > max(1))
                    max.setY(p(1));

                if(p(2) > max(2))
                    max.setZ(p(2));

                if(p(0) < min(0))
                    min.setX(p(0));

                if(p(1) < min(1))
                    min.setY(p(1));

                if(p(2) < min(2))
                    min.setZ(p(2));

                if(rnIndex != NULL) {
                    size_t i = rnIndex->indexOf(itP.cIndex());

                    if(i < rnSize)
                    {
                        const int indexInTab = rn->dValueAt(i)-1;

                        quint64 &val = header->m_numberOfPointsByReturn[indexInTab];
                        ++val;

                        if(indexInTab < 5) {
                            quint32 &legacyVal = header->m_legacyNumberOfPointsByReturn[(int)rn->dValueAt(i)-1];
                            ++legacyVal;
                        }
                    }
                }
            }
        }

        if(rn == NULL) {
            header->m_numberOfPointsByReturn[0] = nPoints;
            header->m_legacyNumberOfPointsByReturn[0] = nPoints;
        }

        QString error;
        QDate date = QDate::currentDate();

        header->m_fileSourceID = 1;
        header->m_globalEncoding = 1;
        header->m_projectIDGuidData1 = 0;
        header->m_projectIDGuidData2 = 0;
        header->m_projectIDGuidData3 = 0;
        header->m_projectIDGuidData4 = 0;
        header->m_versionMajor = 1;  // format 1.2
        header->m_versionMinor = 2;
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

        if(((header->m_versionMinor == 1) && (header->m_versionMajor >= 4)) || (header->m_versionMinor > 1)) {
            header->m_legacyNumberOfPointRecord = 0; // deprecated in 1.4
        } else {
            header->m_legacyNumberOfPointRecord = nPoints;
        }

        // maximum point coordinate
        double scaleFactor = qMax(qMax(qAbs(max(0)), qAbs(max(1))), qAbs(max(2)));
        scaleFactor = qMax(qMax(qMax(qAbs(min(0)), qAbs(min(1))), qAbs(min(2))), scaleFactor);

        // scale factor is the optimized to convert a point to be the most accurate
        scaleFactor = qMin((scaleFactor/(std::numeric_limits<qint32>::max()-1)), 1.0);

        header->m_xScaleFactor = scaleFactor;
        header->m_yScaleFactor = scaleFactor;
        header->m_zScaleFactor = scaleFactor;
        header->m_xOffset = 0;
        header->m_yOffset = 0;
        header->m_zOffset = 0;
        header->setMinAndMaxCoordinates(min(0), min(1), min(2), max(0), max(1), max(2));
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

int CT_Exporter_LAS::getPointDataFormat() const
{
    if(m_lasContainer == NULL)
        return 1;

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

    return 1;
}

size_t CT_Exporter_LAS::getPointDataLength(const int &optFormat) const
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

CT_AbstractLASPointFormat *CT_Exporter_LAS::createPointDataFormat(const int &optFormat) const
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
