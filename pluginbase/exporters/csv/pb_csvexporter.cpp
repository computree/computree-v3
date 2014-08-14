#include "pb_csvexporter.h"

#include "pb_csvexporterconfiguration.h"
#include "views/exporters/csv/pbg_csvconfigurationdialog.h"
#include "pb_csvexportercolumn.h"

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
    return "Export d'attributs (csv)";
}

void PB_CSVExporter::init()
{
    addNewExportFormat(FileFormat("csv", tr("Fichiers csv")));
}

bool PB_CSVExporter::setItemDrawableToExport(const QList<ItemDrawable*> &list)
{
    if(CT_AbstractExporter::setItemDrawableToExport(list))
    {
        QMap<const IItemDataRefList*, QList<ItemDrawable*>* > newMap;

        QListIterator<ItemDrawable*> it(list);

        while(it.hasNext())
        {
            ItemDrawable *item = it.next();

            const IItemDataRefList *refList = item->dataReferencesListStatic();

            if(refList != NULL)
            {
                QList<ItemDrawable*> *list = newMap.value(refList, NULL);

                if(list == NULL)
                {
                    list = new QList<ItemDrawable*>();
                    newMap.insert(refList, list);
                }

                list->append(item);
            }
        }

        _mapKeyChanged = true;

        QMapIterator<const IItemDataRefList*, QList<ItemDrawable*>* > itMap(_mapItemToExport);

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

        return !_mapItemToExport.isEmpty();
    }

    clearMap();
    _mapKeyChanged = true;

    return false;
}

bool PB_CSVExporter::configureExport()
{
    QList<const IItemDataRefList*> list = _mapItemToExport.keys();

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

    dialog.setListOfDataRefList(&list);

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

    QListIterator< QPair<const IItemDataRefList*, const IItemDataRef*> > it(_configuration->getColumns());

    while(it.hasNext())
    {
        const QPair<const IItemDataRefList*, const IItemDataRef*> &pair = it.next();

        SettingsNodeGroup *groupCol = new SettingsNodeGroup("Column");
        groupCol->addValue(new SettingsNodeValue("Version", "1"));
        groupCol->addValue(new SettingsNodeValue("ColumnIndex", i));
        groupCol->addValue(new SettingsNodeValue("IItemDataRefListName", pair.first->name()));
        groupCol->addValue(new SettingsNodeValue("IItemDataRefName", pair.second->name()));

        SettingsNodeGroup *itemDataRefConf = pair.second->saveConfiguration();

        if(itemDataRefConf != NULL)
        {
            SettingsNodeGroup *groupRefConf = new SettingsNodeGroup("IItemDataRefConfiguration");
            groupRefConf->addGroup(itemDataRefConf);
            groupCol->addGroup(groupRefConf);
        }

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
        QList<const IItemDataRefList*> list = _mapItemToExport.keys();

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

            values = column->valuesByTagName("IItemDataRefListName");

            if(values.isEmpty())
                return false;

            const IItemDataRefList *refList = getItemDataRefListByName(values.first()->value().toString());

            if(refList == NULL)
                return false;

            values = column->valuesByTagName("IItemDataRefName");

            if(values.isEmpty())
                return false;

            IItemDataRef *ref = getItemDataRefByName(refList, values.first()->value().toString());

            if(ref == NULL)
                return false;

            groups = column->groupsByTagName("IItemDataRefConfiguration");

            if(!groups.isEmpty())
            {
                if(!ref->loadConfiguration(groups.first()))
                    return false;
            }

            _configuration->addColumn(refList, ref);
        }

        return true;
    }

    return false;
}

IExporter* PB_CSVExporter::copy() const
{
    return new PB_CSVExporter();
}

bool PB_CSVExporter::protectedExportToFile()
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
        QListIterator< QPair<const IItemDataRefList*, const IItemDataRef*> > it(_configuration->getColumns());

        while(it.hasNext())
        {
            const QPair<const IItemDataRefList*, const IItemDataRef*> &column = it.next();
            stream << column.second->displayableName() << ";";

            columns.append(new PB_CSVExporterColumn(*_mapItemToExport.value(column.first)/*itemDrawableToExport()*/, *column.second));
        }

        // ecriture des données
        int currentItemIndex = 0;
        int lastCurrentItemIndex;
        bool hasAtLeastOneNextValue;

        size_t completeSize = 0;
        size_t currentPos = 0;

        QListIterator<PB_CSVExporterColumn*> itC(columns);

        while(itC.hasNext())
        {
            completeSize += itC.next()->size();
        }

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
                    stream << column->nextValueToString() << ";";

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
                    stream << ";";
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

    return false;
}

void PB_CSVExporter::clearMap()
{
    qDeleteAll(_mapItemToExport.begin(), _mapItemToExport.end());
    _mapItemToExport.clear();
}

const IItemDataRefList* PB_CSVExporter::getItemDataRefListByName(const QString &name) const
{
    QMapIterator<const IItemDataRefList*, QList<ItemDrawable*>* > it(_mapItemToExport);

    while(it.hasNext())
    {
        it.next();

        if(it.key()->name() == name)
            return it.key();
    }

    return NULL;
}

IItemDataRef* PB_CSVExporter::getItemDataRefByName(const IItemDataRefList *refList, const QString &name) const
{
    QListIterator<IItemDataRef*> it(refList->references());

    while(it.hasNext())
    {
        IItemDataRef *ref = it.next();

        if(ref->name() == name)
            return ref;
    }

    return NULL;
}
