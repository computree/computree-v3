#include "pb_xybexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"

PB_XYBExporter::PB_XYBExporter() : CT_AbstractExporterPointAttributesSelection()
{
    setCanExportItems(true);
    setCanExportPoints(true);
}

PB_XYBExporter::~PB_XYBExporter()
{
}

QString PB_XYBExporter::getExporterCustomName() const
{
    return "Points (X,Y,Z,I), format xyb";
}

void PB_XYBExporter::init()
{
    addNewExportFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));
}

bool PB_XYBExporter::setItemDrawableToExport(const QList<ItemDrawable*> &list)
{
    QList<ItemDrawable*> myList;
    QListIterator<ItemDrawable*> it(list);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

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

bool PB_XYBExporter::setPointsToExport(const QList<ICloudIndex *> &list)
{
    QList<ICloudIndex*> myList;
    QListIterator<ICloudIndex*> it(list);

    while(it.hasNext())
    {
        ICloudIndex *item = it.next();

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

SettingsNodeGroup* PB_XYBExporter::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporter::saveExportConfiguration();

    /*SettingsNodeGroup *myRoot = new SettingsNodeGroup("PB_XYBExporter");

    if(!m_attributes.isEmpty())
    {
        QListIterator< PBACS > it(m_attributes);

        while(it.hasNext())
        {
            PBACS pas = it.next();
            QListIterator<PBACR> itR(pas.results);

            while(itR.hasNext())
            {
                PBACR par = itR.next();
                QListIterator<IPointAttributes*> itA(par.collection);

                while(itA.hasNext())
                {
                    IPointAttributes *pa = itA.next();

                    SettingsNodeGroup *paG = new SettingsNodeGroup("PointAttributes");

                    SettingsNodeValue *paV = new SettingsNodeValue("step", pas.step->getStepName());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("result", par.result->getModel()->name());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("attributes", pa->getModel()->name());
                    paG->addValue(paV);

                    paV = new SettingsNodeValue("checked", (m_worker.m_attributes.contains(pa) ? "true" : "false"));
                    paG->addValue(paV);

                    myRoot->addGroup(paG);
                }
            }
        }
    }

    root->addGroup(myRoot);*/

    return root;
}

bool PB_XYBExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    /*m_worker.m_attributes.clear();

    QList<SettingsNodeGroup*> groups = root->groupsByTagName("PB_XYBExporter");

    if(groups.isEmpty())
        return false;

    groups = groups.first()->groupsByTagName("PointAttributes");

    QListIterator< SettingsNodeGroup* > itGroups(groups);
    QListIterator< PBACS > it(m_attributes);

    while(it.hasNext())
    {
        PBACS pas = it.next();
        QListIterator<PBACR> itR(pas.results);

        while(itR.hasNext())
        {
            PBACR par = itR.next();
            QListIterator<IPointAttributes*> itA(par.collection);

            while(itA.hasNext())
            {
                if(!itGroups.hasNext())
                    return false;

                SettingsNodeGroup *group = itGroups.next();
                IPointAttributes *pa = itA.next();

                QList<SettingsNodeValue*> values = group->valuesByTagName("result");

                if(values.isEmpty() || (values.first()->value().toString() != par.result->getModel()->name()))
                    return false;

                values = group->valuesByTagName("attributes");

                if(values.isEmpty() || (values.first()->value().toString() != pa->getModel()->name()))
                    return false;

                values = group->valuesByTagName("checked");

                if(values.isEmpty())
                    return false;

                if(values.first()->value().toString() == "true")
                    m_worker.m_attributes.append(pa);
            }
        }
    }*/

    return CT_AbstractExporter::loadExportConfiguration(root);
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
    if(CT_AbstractExporterPointAttributesSelection::useSelection(selectorWidget))
    {
        QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
        QListIterator<CT_AbstractSingularItemDrawable*> it(list);

        while(it.hasNext())
        {
            CT_Scanner *scan = dynamic_cast<CT_Scanner*>(it.next());

            if(scan != NULL)
            {
                m_scanner = scan;
                return true;
            }
        }

        return true;
    }

    return false;
}

IExporter* PB_XYBExporter::copy() const
{
    return new PB_XYBExporter();
}

bool PB_XYBExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "xyb";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "# SCENE XYZ binary format v1.0\n";

        if(m_scanner != NULL)
        {
            txtStream << "ScanPosition " << m_scanner->getPosition().x() << " " << m_scanner->getPosition().y() << " " << m_scanner->getPosition().z() << " \n";
        }
        else
        {
            txtStream << "ScanPosition 0.00000000 0.00000000 0.00000000 \n";
        }

        txtStream << "Rows 0\n";
        txtStream << "Cols 0\n";
        file.close();
    }

    if(file.open(QFile::Append))
    {
        IColorCloud *cc = createColorCloudBeforeExportToFile();

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        char d_data[8];
        quint8 *bgra;

        // write header
        d_data[0] = 0;
        d_data[1] = 0;
        d_data[2] = 0;
        d_data[3] = 0;

        stream.writeRawData(d_data, 4);

        int totalToExport = itemDrawableToExport().size();

        const QList<ICloudIndex*> &pointsSelected = pointsToExport();
        totalToExport = pointsSelected.size();

        int nExported = 0;

        // write data
        QListIterator<ItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            ItemDrawable *item = it.next();

            exportPoints(stream,
                         dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex(),
                         cc,
                         bgra,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        QListIterator<ICloudIndex*> itCI(pointsSelected);

        while(itCI.hasNext())
        {
            exportPoints(stream,
                         dynamic_cast<CT_AbstractCloudIndexT<CT_Point>*>(itCI.next()),
                         cc,
                         bgra,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        clearWorker();

        file.close();
        return true;
    }

    clearWorker();

    return false;
}

void PB_XYBExporter::clearWorker()
{
    CT_AbstractExporterPointAttributesSelection::clearWorker();

    m_scanner = NULL;
}

void PB_XYBExporter::exportPoints(QDataStream &stream,
                                  const CT_AbstractCloudIndexT<CT_Point> *constPCIndex,
                                  const IColorCloud *cc,
                                  quint8 *bgra,
                                  const int &nExported,
                                  const int &totalToExport)
{
    size_t totalSize = constPCIndex->size();
    size_t i = 0;

    CT_AbstractCloudIndexT<CT_Point>::ConstIterator begin = constPCIndex->constBegin();
    CT_AbstractCloudIndexT<CT_Point>::ConstIterator end = constPCIndex->constEnd();

    while(begin != end)
    {
        const CT_Point &point = begin.cT();

        stream << ((double)point.data[0]);
        stream << ((double)point.data[1]);
        stream << ((double)point.data[2]);

        if(cc == NULL)
        {
            stream << (quint16)0;
        }
        else
        {
            bgra = cc->valueAt(begin.cIndex());
            quint16 tmp = (quint16)bgra[0];
            stream << tmp;
        }

        ++begin;
        ++i;

        setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
    }
}

