#include "pb_csvexporter.h"

#include "pb_csvexporterconfiguration.h"
#include "views/exporters/csv/pbg_csvconfigurationdialog.h"
#include "pb_csvexportercolumn.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <limits>
#include <QFileInfo>

PB_CSVExporter::PB_CSVExporter() : CT_AbstractExporter()
{
    _configuration = NULL;
    _mapKeyChanged = true;
}

PB_CSVExporter::~PB_CSVExporter()
{
    delete _configuration;
    clearMap();
}

QString PB_CSVExporter::getExporterCustomName() const
{
    return tr("Export d'attributs (csv)");
}

void PB_CSVExporter::init()
{
    addNewExportFormat(FileFormat("csv", tr("Fichiers csv")));
}

bool PB_CSVExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_AbstractSingularItemDrawable*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun Singular Item"));
        return false;
    }

    if(CT_AbstractExporter::setItemDrawableToExport(myList))
    {
        QMap<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > newMap;

        it = myList;

        while(it.hasNext())
        {
            CT_AbstractSingularItemDrawable *item = dynamic_cast<CT_AbstractSingularItemDrawable*>(it.next());

            QList<CT_AbstractItemDrawable*> *list = newMap.value((CT_OutAbstractSingularItemModel*)item->model(), NULL);

            if(list == NULL)
            {
                list = new QList<CT_AbstractItemDrawable*>();
                newMap.insert((CT_OutAbstractSingularItemModel*)item->model(), list);
            }

            list->append(item);
        }

        _mapKeyChanged = true;

        QMapIterator<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > itMap(_mapItemToExport);

        if(itMap.hasNext())
        {
            _mapKeyChanged = false;

            while(itMap.hasNext()
                  && !_mapKeyChanged)
            {
                if(!newMap.contains(itMap.next().key()))
                    _mapKeyChanged = true;
            }
        }

        _mapItemToExport = newMap;

        configureExportWithLastConfigurationAndNewItemToExport();

        return !_mapItemToExport.isEmpty();
    }

    clearMap();
    _mapKeyChanged = true;

    return false;
}

bool PB_CSVExporter::configureExport()
{
    QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

    if(_mapKeyChanged
            || (_configuration == NULL))
    {
        delete _configuration;
        _configuration = new PB_CSVExporterConfiguration(list);
    }
    else
    {
        _configuration->setList(list);
    }

    PBG_CSVConfigurationDialog dialog(*_configuration);

    bool ret = (dialog.exec() == QDialog::Accepted) && (!_configuration->getColumns().isEmpty());

    _mapKeyChanged = false;

    return ret;
}

SettingsNodeGroup* PB_CSVExporter::saveExportConfiguration() const
{
    if(_configuration == NULL)
        return NULL;

    SettingsNodeGroup *root = CT_AbstractExporter::saveExportConfiguration();
    SettingsNodeGroup *group = new SettingsNodeGroup("PB_CSVExporter");
    group->addValue(new SettingsNodeValue("Version", "1"));

    SettingsNodeGroup *groupC = new SettingsNodeGroup("Configuration");
    group->addGroup(groupC);

    int i = 0;

    QListIterator< QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > it(_configuration->getColumns());

    while(it.hasNext())
    {
        const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &pair = it.next();

        SettingsNodeGroup *groupCol = new SettingsNodeGroup("Column");
        groupCol->addValue(new SettingsNodeValue("Version", "1"));
        groupCol->addValue(new SettingsNodeValue("ColumnIndex", i));
        groupCol->addValue(new SettingsNodeValue("ItemModelUniqueName", pair.first->uniqueName()));
        groupCol->addValue(new SettingsNodeValue("ItemAttributeModelUniqueName", pair.second->uniqueName()));

        groupC->addGroup(groupCol);

        ++i;
    }

    root->addGroup(group);

    return root;
}

bool PB_CSVExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    if(CT_AbstractExporter::loadExportConfiguration(root))
    {
        QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

        delete _configuration;
        _configuration = new PB_CSVExporterConfiguration(list);

        QList<SettingsNodeGroup*> groups = root->groupsByTagName("PB_CSVExporter");

        if(groups.isEmpty())
            return false;

        groups = groups.first()->groupsByTagName("Configuration");

        if(groups.isEmpty())
            return false;

        groups = groups.first()->groupsByTagName("Column");

        QVector<SettingsNodeGroup*> columns(groups.size(), NULL);

        QList<SettingsNodeValue*> values;
        QListIterator<SettingsNodeGroup*> it(groups);

        while(it.hasNext())
        {
            SettingsNodeGroup *column = it.next();
            values = column->valuesByTagName("ColumnIndex");

            if(values.isEmpty())
                return false;

            bool ok;
            int index = values.first()->value().toInt(&ok);

            if(!ok)
                return false;

            columns[index] = column;
        }

        // vérifie qu'on a bien toute les colonnes
        QVectorIterator<SettingsNodeGroup*> itV(columns);

        while(itV.hasNext())
        {
            if(itV.next() == NULL)
                return false;
        }

        itV.toFront();

        while(itV.hasNext())
        {
            SettingsNodeGroup *column = itV.next();

            values = column->valuesByTagName("ItemModelUniqueName");

            if(values.isEmpty())
                return false;

            CT_OutAbstractSingularItemModel *refList = getItemModelByName(values.first()->value().toString());

            if(refList == NULL)
                return false;

            values = column->valuesByTagName("ItemAttributeModelUniqueName");

            if(values.isEmpty())
                return false;

            CT_OutAbstractItemAttributeModel *ref = getItemAttributeModelByName(refList, values.first()->value().toString());

            if(ref == NULL)
                return false;

            _configuration->addColumn(refList, ref);
        }

        return true;
    }

    return false;
}

CT_AbstractExporter* PB_CSVExporter::copy() const
{
    return new PB_CSVExporter();
}

bool PB_CSVExporter::protectedExportToFile()
{
    if(!_mapKeyChanged)
    {
        QFileInfo exportPathInfo = QFileInfo(exportFilePath());
        QString path = exportPathInfo.path();
        QString baseName = exportPathInfo.baseName();
        QString suffix = "csv";
        QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            QList<PB_CSVExporterColumn*> columns;

            // ecriture de l'header
            QListIterator< QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > it(_configuration->getColumns());

            while(it.hasNext())
            {
                const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = it.next();

                QString dispName = column.second->displayableName() .remove(" ")
                                                                    .remove('\'')
                                                                    .remove('\"')
                                                                    .remove('\t')
                                                                    .remove(';')
                                                                    .remove(':')
                                                                    .remove(',')
                                                                    .remove('.');
                stream << dispName;

                if (it.hasNext()) {stream << ";";}

                QList<CT_AbstractItemDrawable*> *lToConvert = _mapItemToExport.value(column.first);
                QList<CT_AbstractSingularItemDrawable*> lToPass;

                foreach (CT_AbstractItemDrawable *itemD, *lToConvert) {
                    lToPass.append((CT_AbstractSingularItemDrawable*)itemD);
                }

                columns.append(new PB_CSVExporterColumn(lToPass, column.second));
            }

            // ecriture des données
            int currentItemIndex = 0;
            int lastCurrentItemIndex;
            bool hasAtLeastOneNextValue;

            size_t completeSize = 0;
            size_t currentPos = 0;

            QListIterator<PB_CSVExporterColumn*> itC(columns);

            while(itC.hasNext())
                completeSize += itC.next()->size();

            do
            {
                stream << "\r\n";

                itC.toFront();
                lastCurrentItemIndex = std::numeric_limits<int>::max();
                hasAtLeastOneNextValue = false;

                // pour chaque colonne
                while(itC.hasNext())
                {
                    PB_CSVExporterColumn *column = itC.next();

                    // si cette colonne va exporter une valeur
                    // d'un item dont l'index dans la liste est le même
                    // que les autres colonnes
                    if((column->currentItemIndex() == currentItemIndex)
                            && column->hasNextValue())
                    {
                        // on exporte la valeur
                        stream << column->nextValueToString();

                        if (itC.hasNext()) {stream << ";";}

                        // on garde en mémoire l'index de l'item
                        if(column->currentItemIndex() < lastCurrentItemIndex)
                            lastCurrentItemIndex = column->currentItemIndex();

                        // si il y aura encore une valeur a exporter
                        if(column->hasNextValue()
                                && !hasAtLeastOneNextValue)
                            hasAtLeastOneNextValue = true;


                        ++currentPos;
                        setExportProgress((currentPos*95)/completeSize);
                    }
                    else
                    {
                        if (itC.hasNext()) {stream << ";";}
                    }
                }

                currentItemIndex = lastCurrentItemIndex;

            // tant qu'il reste au moins une valeur à exporter
            }while(hasAtLeastOneNextValue);

            qDeleteAll(columns.begin(), columns.end());

            file.close();
            setExportProgress(100);

            return true;
        }
    }

    return false;
}

bool PB_CSVExporter::configureExportWithLastConfigurationAndNewItemToExport()
{
    if(_mapKeyChanged && (_configuration != NULL))
    {
        QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

        QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > columns = _configuration->getColumns();
        QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > newColumns = columns;
        QVector<bool> replaced(columns.size());

        QListIterator<CT_OutAbstractSingularItemModel*> it(list);

        while(it.hasNext())
        {
            CT_OutAbstractSingularItemModel *model = it.next();
            QListIterator<CT_OutAbstractItemAttributeModel*> itA(model->itemAttributes());

            int index = 0;

            QListIterator<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > itC(columns);

            while(itC.hasNext())
            {
                const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = itC.next();

                // if same original model
                if(column.first->originalModel() == model->originalModel())
                {
                    itA.toFront();

                    bool ok = false;
                    while(itA.hasNext()
                          && !ok)
                    {
                        CT_OutAbstractItemAttributeModel *attModel = itA.next();

                        // if same original model
                        if(column.second->originalModel() == attModel->originalModel())
                        {
                            // we can replace this pair by this model
                            replaced.replace(index, true);

                            newColumns.replace(index, qMakePair(model, attModel));

                            ok = true;
                        }
                    }
                }

                ++index;
            }
        }

        QVectorIterator<bool> itV(replaced);

        while(itV.hasNext())
        {
            if(itV.next() == false)
                return false;
        }

        _configuration->clearColumns();

        QListIterator<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > itC(newColumns);

        while(itC.hasNext())
        {
            const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = itC.next();

            _configuration->addColumn(column.first, column.second);
        }

        _mapKeyChanged = false;
    }

    return !_mapKeyChanged;
}

void PB_CSVExporter::clearMap()
{
    qDeleteAll(_mapItemToExport.begin(), _mapItemToExport.end());
    _mapItemToExport.clear();
}

CT_OutAbstractSingularItemModel* PB_CSVExporter::getItemModelByName(const QString &name) const
{
    QMapIterator<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > it(_mapItemToExport);

    while(it.hasNext())
    {
        it.next();

        if(it.key()->uniqueName() == name)
            return it.key();
    }

    return NULL;
}

CT_OutAbstractItemAttributeModel* PB_CSVExporter::getItemAttributeModelByName(CT_OutAbstractSingularItemModel *sItem, const QString &name) const
{
    QListIterator<CT_OutAbstractItemAttributeModel*> it(sItem->itemAttributes());

    while(it.hasNext())
    {
        CT_OutAbstractItemAttributeModel *ref = it.next();

        if(ref->uniqueName() == name)
            return ref;
    }

    return NULL;
}
