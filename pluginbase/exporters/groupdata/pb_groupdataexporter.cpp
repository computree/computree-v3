#include "pb_groupdataexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include <QDebug>

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemdrawablemodel.h"

PB_GroupDataExporter::PB_GroupDataExporter() : CT_AbstractExporter()
{
}

PB_GroupDataExporter::~PB_GroupDataExporter()
{

}

QString PB_GroupDataExporter::getExporterCustomName() const
{
    return "Attributs/Items d'un groupe";
}

void PB_GroupDataExporter::init()
{
    addNewExportFormat(FileFormat("txt", tr("Fichier txt")));
}

bool PB_GroupDataExporter::configureExport()
{
    return true;
}

bool PB_GroupDataExporter::setItemDrawableToExport(const QList<ItemDrawable*> &list)
{
    clearErrorMessage();

    QList<ItemDrawable*> myList;
    QListIterator<ItemDrawable*> it(list);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(dynamic_cast<CT_AbstractItemGroup*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun Groupe"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

IExporter* PB_GroupDataExporter::copy() const
{
    return new PB_GroupDataExporter();
}

bool PB_GroupDataExporter::protectedExportToFile()
{
    bool exportCompleted = true;

    // Création d'une map de référence des modèles de groupes / modèles d'items / dataRéférences
    // QMap<Modèle de groupe, QMapMultiMap<Modèle d'item, dataRéférence> >
    // + une map listant les groupes par modèle
    // + une map pour les headers de fichiers (un par modèle de groupe)
    QMap<IItemModel*, QMultiMap<IItemModel*, IItemDataRef*>* > dataRefIndex;
    QMultiMap<IItemModel*, CT_AbstractItemGroup*> groupMap;

    // Parcours des groupes
    QListIterator<ItemDrawable*> it_itemsToExport(itemDrawableToExport());
    while(it_itemsToExport.hasNext())
    {
        CT_AbstractItemGroup* group = dynamic_cast<CT_AbstractItemGroup*>(it_itemsToExport.next());

        if (group !=NULL)
        {
            IItemModel *groupModel = group->getModel();
            groupMap.insert(groupModel, group);

            QMultiMap<IItemModel*, IItemDataRef*> *itemMap = dataRefIndex.value(groupModel, NULL);

            // Si ce modèle de groupe n'a pas encore été rencontré : on l'ajoute
            if (itemMap == NULL)
            {
                itemMap = new QMultiMap<IItemModel*, IItemDataRef*>();
                dataRefIndex.insert(groupModel, itemMap);

                // Liste des modèles d'items contenus dans le modèle de groupe
                QList<IItemModel*> itemModels = groupModel->getChildrenItemModel();

                QListIterator<IItemModel*> it_itemsModels(itemModels);
                while (it_itemsModels.hasNext())
                {
                    IItemModel* itemModel = it_itemsModels.next();
                    ItemDrawable *item = itemModel->itemDrawable();

                    // On utilise l'item "modèle" pour accéder aux dataRef scalaires disponibles
                    if (item != NULL && dynamic_cast<CT_AbstractSingularItemDrawable*>(item) != NULL)
                    {
                        const QList<IItemDataRef*> &dataRefList = item->dataReferencesListStatic()->references();

                        QListIterator<IItemDataRef*> it_dataRefList(dataRefList);
                        it_dataRefList.toBack();
                        while (it_dataRefList.hasPrevious())
                        {
                            IItemDataRef* dataRef = it_dataRefList.previous();
                            IItemDataValue::ItemDataValueType dataType = dataRef->dataType();

                            if (dataType >= IItemDataValue::IDVT_BOOL && dataType < IItemDataValue::IDVT_UNKNOWN_SCALAR)
                            {
                                itemMap->insert(itemModel, dataRef);
                            }
                        }
                    }
                }
            }
        }
    }



    // Variables recyclées
    IItemDataValue *valueToSet = PS_REPOSITORY->createNewEmptyDataValue();
    bool ok;

    QFileInfo fileInfo = QFileInfo(exportFilePath());
    QString basePath = QString("%1/%2").arg(fileInfo.absolutePath()).arg(fileInfo.baseName());
    QString suffix = ".txt";

    bool multiFiles = (dataRefIndex.keys().size() > 1);

    int groupModelRank = 1;



    // Parcours des modèles de groupes : pour chacun un fichier de sortie
    QMapIterator<IItemModel*, QMultiMap<IItemModel*, IItemDataRef*>* > it_dataRefIndex(dataRefIndex);
    while (it_dataRefIndex.hasNext())
    {
        it_dataRefIndex.next();

        // Modèle de groupe
        IItemModel *groupModel = it_dataRefIndex.key();
        QMultiMap<IItemModel*, IItemDataRef*> *itemMap = it_dataRefIndex.value();

        if (multiFiles) {suffix = QString("_%1_%2.txt").arg(groupModelRank++).arg(groupModel->displayableName());}
        QFile file(QString("%1%2").arg(basePath).arg(suffix));

        // Ouverture du fichier d'export correspondant au modèle de groupe en cours
        if(file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream txtStream(&file);


            // Création du header
            QString header_line1, header_line2;
            // Liste des modèles d'items du modèle de groupe
            QList<IItemModel*> itemModelList = itemMap->uniqueKeys();

            QListIterator<IItemModel*> it_itemModelList(itemModelList);
            while (it_itemModelList.hasNext())
            {
                IItemModel *itemModel = it_itemModelList.next();

                // Liste des dataRef du modèle d'item en cours
                QList<IItemDataRef*> dataRefList = itemMap->values(itemModel);

                QListIterator<IItemDataRef*> it_dataRefList(dataRefList);
                while (it_dataRefList.hasNext())
                {
                    IItemDataRef* dataRef = it_dataRefList.next();
                    header_line1.append(itemModel->displayableName());
                    header_line1.append("\t");

                    header_line2.append(dataRef->displayableName());
                    header_line2.append("\t");
                }
            }
            txtStream << header_line1 << "\n";
            txtStream << header_line2 << "\n";


            // Liste des groupes correspondant au modèle de groupe en cours
            QList<CT_AbstractItemGroup*> groupListForThisModel = groupMap.values(groupModel);

            QListIterator<CT_AbstractItemGroup*> it_Groups(groupListForThisModel);
            while(it_Groups.hasNext())
            {
                const CT_AbstractItemGroup* group = it_Groups.next();

                // Liste des modèles d'items du modèle de groupe
                QList<IItemModel*> itemModelList = itemMap->uniqueKeys();

                QListIterator<IItemModel*> it_itemModelList(itemModelList);
                while (it_itemModelList.hasNext())
                {
                    IItemModel *itemModel = it_itemModelList.next();

                    // Liste des dataRef du modèle d'item en cours
                    QList<IItemDataRef*> dataRefList = itemMap->values(itemModel);

                    // Récupération de l'item correspondant au modèle d'item
                    CT_AbstractSingularItemDrawable* item = group->findFirstItem((CT_OutAbstractItemDrawableModel*) itemModel);

                    QListIterator<IItemDataRef*> it_dataRefList(dataRefList);
                    while (it_dataRefList.hasNext())
                    {
                        IItemDataRef* dataRef = it_dataRefList.next();

                        // Si l'item existe, on exporte les dataRef
                        if (item != NULL)
                        {
                            item->dataValueFromRef(*dataRef, valueToSet);
                            QString value = valueToSet->toString(&ok);

                            if (ok) {txtStream << value;}
                        }
                        // Dans tous les cas on change de colonne
                        txtStream << "\t";
                    }
                }

                // Après chaque groupe on saute une ligne
                txtStream << "\n";
            }

            // On ferme le fichier pour ce modèle de groupe
            file.close();
        } else {exportCompleted = false;} // Si au moins un fichier a raté => renvoie false
    }

    // Suppression des pointeurs créés
    qDeleteAll(dataRefIndex.values());
    delete valueToSet;

    return exportCompleted;
}
