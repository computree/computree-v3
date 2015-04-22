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
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

PB_GroupDataExporter::PB_GroupDataExporter() : CT_AbstractExporter()
{
    setExportOnlyGroup(true);
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

bool PB_GroupDataExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

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

CT_AbstractExporter* PB_GroupDataExporter::copy() const
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
    QMap<CT_OutAbstractGroupModel*, QMultiMap<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*>* > itemAttModelIndex;
    QMultiMap<CT_OutAbstractGroupModel*, CT_AbstractItemGroup*> groupMap;

    // Parcours des groupes
    QListIterator<CT_AbstractItemDrawable*> it_itemsToExport(itemDrawableToExport());
    while(it_itemsToExport.hasNext())
    {
        CT_AbstractItemGroup* group = dynamic_cast<CT_AbstractItemGroup*>(it_itemsToExport.next());

        CT_OutAbstractGroupModel *groupModel = (CT_OutAbstractGroupModel*)group->model();
        groupMap.insert(groupModel, group);

        QMultiMap<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*> *itemMap = itemAttModelIndex.value(groupModel, NULL);

        // Si ce modèle de groupe n'a pas encore été rencontré : on l'ajoute
        if (itemMap == NULL)
        {
            itemMap = new QMultiMap<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*>();
            itemAttModelIndex.insert(groupModel, itemMap);

            // Liste des modèles d'items contenus dans le modèle de groupe
            QList<CT_AbstractModel*> itemModels = groupModel->childrens();

            QListIterator<CT_AbstractModel*> it_itemsModels(itemModels);
            while (it_itemsModels.hasNext())
            {
                CT_OutAbstractSingularItemModel* itemModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(it_itemsModels.next());

                // Si le modèle contient un SingularItemDrawable
                if(itemModel != NULL)
                {
                    // On récupère la liste des attributs
                    const QList<CT_OutAbstractItemAttributeModel*> &itemAttributeList = itemModel->itemAttributes();

                    QListIterator<CT_OutAbstractItemAttributeModel*> it_itemAttributeList(itemAttributeList);
                    it_itemAttributeList.toBack();

                    // et on les ajoute à la map
                    while (it_itemAttributeList.hasPrevious())
                        itemMap->insert(itemModel, it_itemAttributeList.previous());
                }
            }
        }
    }



    // Variables recyclées
    bool ok;

    QFileInfo fileInfo = QFileInfo(exportFilePath());
    QString basePath = QString("%1/%2").arg(fileInfo.absolutePath()).arg(fileInfo.baseName());
    QString suffix = ".txt";

    bool multiFiles = (itemAttModelIndex.keys().size() > 1);

    int groupModelRank = 1;

    // Parcours des modèles de groupes : pour chacun un fichier de sortie
    QMapIterator<CT_OutAbstractGroupModel*, QMultiMap<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*>* > it_itemAttModelIndex(itemAttModelIndex);
    while (it_itemAttModelIndex.hasNext())
    {
        it_itemAttModelIndex.next();

        // Modèle de groupe
        CT_OutAbstractGroupModel *groupModel = it_itemAttModelIndex.key();
        QMultiMap<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*> *itemMap = it_itemAttModelIndex.value();

        if (multiFiles) {suffix = QString("_%1_%2.txt").arg(groupModelRank++).arg(groupModel->displayableName());}
        QFile file(QString("%1%2").arg(basePath).arg(suffix));

        // Ouverture du fichier d'export correspondant au modèle de groupe en cours
        if(file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream txtStream(&file);


            // Création du header
            QString header_line1, header_line2;

            header_line1.append("GroupID\t");
            header_line2.append("GroupID\t");
            header_line1.append("ParentID\t");
            header_line2.append("ParentID\t");

            // Liste des modèles d'items du modèle de groupe
            QList<CT_OutAbstractSingularItemModel*> itemModelList = itemMap->uniqueKeys();

            QListIterator<CT_OutAbstractSingularItemModel*> it_itemModelList(itemModelList);
            while (it_itemModelList.hasNext())
            {
                CT_OutAbstractSingularItemModel *itemModel = it_itemModelList.next();

                // Liste des attributs du modèle d'item en cours
                QList<CT_OutAbstractItemAttributeModel*> itemAttModelList = itemMap->values(itemModel);

                QListIterator<CT_OutAbstractItemAttributeModel*> it_itemAttModelList(itemAttModelList);
                while (it_itemAttModelList.hasNext())
                {
                    CT_OutAbstractItemAttributeModel* itemAttModel = it_itemAttModelList.next();
                    header_line1.append(itemModel->displayableName());
                    header_line1.append("\t");

                    header_line2.append(itemAttModel->displayableName());
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

                txtStream << group->id() << "\t";

                const CT_AbstractItemGroup* parentGroup = group->parentGroup();
                if (parentGroup != NULL)
                {
                    txtStream << parentGroup->id() << "\t";
                } else {
                    txtStream << "\t";
                }

                // Liste des modèles d'items du modèle de groupe
                QList<CT_OutAbstractSingularItemModel*> itemModelList = itemMap->uniqueKeys();

                QListIterator<CT_OutAbstractSingularItemModel*> it_itemModelList(itemModelList);
                while (it_itemModelList.hasNext())
                {
                    CT_OutAbstractSingularItemModel *itemModel = it_itemModelList.next();

                    // Liste des modèles des attributs d'item du modèle d'item en cours
                    QList<CT_OutAbstractItemAttributeModel*> itemAttModelList = itemMap->values(itemModel);

                    // Récupération de l'item correspondant au modèle d'item
                    CT_AbstractSingularItemDrawable* item = group->item(itemModel);

                    QListIterator<CT_OutAbstractItemAttributeModel*> it_itemAttModelList(itemAttModelList);
                    while (it_itemAttModelList.hasNext())
                    {
                        CT_OutAbstractItemAttributeModel* itemAttModel = it_itemAttModelList.next();

                        // Si l'item existe, on exporte les attributs
                        if (item != NULL)
                        {
                            CT_AbstractItemAttribute *att = item->itemAttribute(itemAttModel);

                            if(att != NULL)
                            {
                                QString value = att->toString(item, &ok);
                                if (ok) {txtStream << value;}
                            }
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
    qDeleteAll(itemAttModelIndex.values());

    return exportCompleted;
}
