#include "pb_xybexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_point.h"
#include "ct_itemdrawable/tools/ct_itemsearchhelper.h"
#include "ct_model/tools/ct_modelsaverestorehelper.h"

PB_XYBExporter::PB_XYBExporter() : CT_AbstractExporterPointAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(true);
    setCanExportWithColors(true);
    setCanExportWithNormals(false);

    m_scanner = NULL;
    m_scannerModel = NULL;
    _file = NULL;
}

PB_XYBExporter::~PB_XYBExporter()
{
}

QString PB_XYBExporter::getExporterCustomName() const
{
    return tr("Points, XYB (X,Y,Z,I)");
}

CT_StepsMenu::LevelPredefined PB_XYBExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}


void PB_XYBExporter::init()
{
    addNewExportFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));

    setToolTip(tr("Export des points dans un fichier format binaire XYB (FARO)"));
}

bool PB_XYBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
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

bool PB_XYBExporter::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
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

SettingsNodeGroup* PB_XYBExporter::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporterPointAttributesSelection::saveExportConfiguration();

    SettingsNodeGroup *myRoot = new SettingsNodeGroup("PB_XYBExporter");

    if(m_scannerModel != NULL) {

        CT_ModelSaveRestoreHelper helper;
        myRoot->addGroup(helper.saveToSearchOutModel(m_scannerModel, "ScannerModel"));
    }

    root->addGroup(myRoot);

    return root;
}

bool PB_XYBExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    clearWorker();

    if(CT_AbstractExporterPointAttributesSelection::loadExportConfiguration(root))
    {
        QList<SettingsNodeGroup*> groups = root->groupsByTagName("PB_XYBExporter");

        if(groups.isEmpty())
            return true;

        groups = groups.first()->groupsByTagName("ScannerModel");

        if(groups.isEmpty())
            return true;

        CT_ModelSaveRestoreHelper helper;
        m_scannerModel = dynamic_cast<CT_OutAbstractSingularItemModel *>(helper.searchModelFromSettings(groups.first(), myStep()));

        return (m_scannerModel != NULL);
    }

    return false;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > PB_XYBExporter::getBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l = CT_AbstractExporterPointAttributesSelection::getBuilders();

    m_scannerBuilderIndex = l.size();
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(tr("Scanners"), (CT_AbstractItemDrawableCollectionBuilder*)&m_scannerBuilder);

    return l;
}

void PB_XYBExporter::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    CT_AbstractExporterPointAttributesSelection::setExcludeConfiguration(pair, model);

    // scanners
    if((&m_scannerBuilder) == pair.second)
    {
        model->addExcludeModel(m_scannerBuilderIndex); // exclude me
    }
}

bool PB_XYBExporter::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearWorker();

    if(CT_AbstractExporterPointAttributesSelection::useSelection(selectorWidget))
    {
        QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
        QListIterator<CT_AbstractSingularItemDrawable*> it(list);

        while(it.hasNext() && (m_scanner == NULL))
            m_scanner = dynamic_cast<CT_Scanner*>(it.next());

        // the scanner can not be found if this exporter is used in a step
        if(m_scanner == NULL) {

            // search the model to use
            QList<CT_OutAbstractSingularItemModel*> listM = selectorWidget->itemDrawableModelSelected();
            QListIterator<CT_OutAbstractSingularItemModel*> itM(listM);

            m_scanner = NULL;

            while(itM.hasNext() && (m_scannerModel == NULL)) {
                CT_OutAbstractSingularItemModel *model = itM.next();
                if(dynamic_cast<CT_Scanner*>(model->itemDrawable()) != NULL)
                    m_scannerModel = model;
            }
        }

        return true;
    }

    return false;
}

CT_AbstractExporter* PB_XYBExporter::copy() const
{
    return new PB_XYBExporter();
}

bool PB_XYBExporter::createExportFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "xyb";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    // if we don't have the scanner but his model
    if(m_scannerModel != NULL) {

        // we search the real scanner
        CT_ItemSearchHelper helper;
        CT_ResultItemIterator it = helper.searchSingularItemsForModel(m_scannerModel);

        if(it.hasNext())
            m_scanner = dynamic_cast<CT_Scanner*>((CT_AbstractSingularItemDrawable*)it.next());
    }

    _file = new QFile(filePath);

    if(_file->open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(_file);

        txtStream << "# SCENE XYZ binary format v1.0\n";

        if(m_scanner != NULL)
        {
            double x = m_scanner->getPosition().x();
            double y = m_scanner->getPosition().y();
            double z = m_scanner->getPosition().z();

            txtStream << "ScanPosition " << x << " " << y << " " << z << "\n";
            txtStream << "Rows " << (m_scanner->getVFov()*RAD2DEG)/(m_scanner->getHRes()*RAD2DEG) << "\n";
            txtStream << "Cols " << (m_scanner->getHFov()*RAD2DEG)/(m_scanner->getVRes()*RAD2DEG) << "\n";
        }
        else
        {
            txtStream << "ScanPosition 0.00000000 0.00000000 0.00000000 \n";
            txtStream << "Rows 0\n";
            txtStream << "Cols 0\n";
        }

        _file->close();
    } else {
        return false;
    }

    if(_file->open(QFile::Append))
    {
        QDataStream stream(_file);
        stream.setByteOrder(QDataStream::LittleEndian);

        char d_data[8];

        // write header
        d_data[0] = 0;
        d_data[1] = 0;
        d_data[2] = 0;
        d_data[3] = 0;

        stream.writeRawData(d_data, 4);

        _file->close();
        return true;
    } else {
        return false;
    }

    return true;
}

bool PB_XYBExporter::exportPointsToFile(CT_AbstractPointCloudIndex *indexVector)
{
    if(_file->open(QFile::Append))
    {
        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();

        QDataStream stream(_file);
        stream.setByteOrder(QDataStream::LittleEndian);

        int totalToExport = 1;
        int nExported = 0;

        exportPoints(stream, indexVector, cc, nExported,  totalToExport);

        _file->close();
        return true;
    }
    return false;
}

void PB_XYBExporter::finalizeExportFile()
{
    // Nothing to do in this case
}

bool PB_XYBExporter::protectedExportToFile()
{
    if (!createExportFile()) {return false;}

    if(_file->open(QFile::Append))
    {
        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();

        QDataStream stream(_file);
        stream.setByteOrder(QDataStream::LittleEndian);

        int totalToExport = itemDrawableToExport().size();

        const QList<CT_AbstractCloudIndex*> &pointsSelected = pointsToExport();

        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            exportPoints(stream,
                         dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex(),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        QListIterator<CT_AbstractCloudIndex*> itCI(pointsSelected);
        totalToExport = pointsSelected.size();

        while(itCI.hasNext())
        {
            exportPoints(stream,
                         dynamic_cast<CT_AbstractPointCloudIndex*>(itCI.next()),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        _file->close();
        return true;
    }

    return false;
}

void PB_XYBExporter::clearWorker()
{
    CT_AbstractExporterPointAttributesSelection::clearWorker();

    m_scanner = NULL;
    m_scannerModel = NULL;
}

void PB_XYBExporter::exportPoints(QDataStream &stream,
                                  const CT_AbstractPointCloudIndex *constPCIndex,
                                  const CT_AbstractColorCloud *cc,
                                  const int &nExported,
                                  const int &totalToExport)
{
    size_t i = 0;

    CT_PointIterator it(constPCIndex);
    size_t totalSize = it.size();

    while(it.hasNext())
    {
        const CT_Point &point = it.next().currentPoint();

        stream << point(CT_Point::X);
        stream << point(CT_Point::Y);
        stream << point(CT_Point::Z);

        if(cc == NULL)
        {
            stream << (quint16)0;
        }
        else
        {
            const CT_Color &col = cc->constColorAt(it.cIndex());
            quint16 tmp = (quint16)col.r();
            stream << tmp;
        }

        ++i;

        setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
    }
}

