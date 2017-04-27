#include "pb_stepexportitemlist.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_turn/inTurn/tools/ct_inturnmanager.h"

#include "ct_itemdrawable/ct_profile.h"
#include "exporters/profile/pb_profileexporter.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_itemdrawable/ct_loopcounter.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_item "item"

#define DEFin_resCounter "resCounter"
#define DEF_inGroupCounter "GroupCounter"
#define DEF_inCounter "counter"

#define DEF_inresName "resName"
#define DEF_inGroupName "GroupName"
#define DEF_inName "Name"
#define DEF_inNameAtt "NameAtt"


#define DEFout_res "res"
#define DEFout_grp "grp"




// Constructor : initialization of parameters
PB_StepExportItemList::PB_StepExportItemList(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _prefixFileName = "";


    // Create the available exporter map
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();
    int s = pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);

        QList<CT_StandardExporterSeparator*> rsl = p->getExportersAvailable();
        QListIterator<CT_StandardExporterSeparator*> itR(rsl);

        while(itR.hasNext())
        {
            CT_StandardExporterSeparator *rs = itR.next();
            QListIterator<CT_AbstractExporter*> itE(rs->exporters());

            while(itE.hasNext())
            {
                CT_AbstractExporter *exporter = itE.next();
                CT_AbstractExporter *exporterCpy = exporter->copy();
                exporterCpy->init();
                _exportersInstancesList.append(exporterCpy);

                const QList<FileFormat>& formats = exporterCpy->exportFormats();

                for (int n = 0 ; n < formats.size() ; n++)
                {
                    const FileFormat& format = formats.at(n);

                    QString key = QString("%2 - %1").arg(exporter->getExporterName()).arg(format.description());
                    _exportersMap.insert(key, exporterCpy);
                }

            }
        }
    }
}

PB_StepExportItemList::~PB_StepExportItemList()
{
    qDeleteAll(_exportersInstancesList);
}


// Step description (tooltip of contextual menu)
QString PB_StepExportItemList::getStepDescription() const
{
    return tr("Export avec nom de fichier adaptatif (DEPRECATED)");
}

// Step detailled description
QString PB_StepExportItemList::getStepDetailledDescription() const
{
    return tr("Permet un export avec nom de fichier adaptatif.<br>"
              "Cette étape peut utiliser n'importe quel exporter.<br>"
              "Le nom du fichier de sorti, est déterminé à partir du compteur de boucle spécifié.");
}

// Step URL
QString PB_StepExportItemList::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExportItemList::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExportItemList(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExportItemList::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn->addItemModel(DEFin_grp, DEFin_item, CT_AbstractItemDrawable::staticGetType(), tr("Item à exporter"));

    CT_InResultModelGroup* resCounter = createNewInResultModel(DEFin_resCounter, tr("Résultat compteur"), "", true);
    resCounter->setRootGroup(DEF_inGroupCounter);
    resCounter->addItemModel(DEF_inGroupCounter, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));

    CT_InResultModelGroup* baseNameRes = createNewInResultModel(DEF_inresName, tr("Nom de base (optionnel)"), "", true);
    baseNameRes->setZeroOrMoreRootGroup();
    baseNameRes->addGroupModel("", DEF_inGroupName, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    baseNameRes->addItemModel(DEF_inGroupName, DEF_inName, CT_AbstractSingularItemDrawable::staticGetType(), tr("Base Name"));
    baseNameRes->addItemAttributeModel(DEF_inName, DEF_inNameAtt, QList<QString>() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Name"));
    baseNameRes->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
}

//bool PB_StepExportItemList::configureInputResult()
//{
//    if(CT_VirtualAbstractStep::configureInputResult())
//        return configureExporter();

//    return false;
//}

//bool PB_StepExportItemList::configureExporter()
//{
//    configureExporterFromModel();

//    if(_exporterConfiguration != NULL)
//        _exporter->loadExportConfiguration(_exporterConfiguration);

//    if(_exporter->configureExport())
//    {
//        clearExporterConfiguration();
//        _exporterConfiguration = _exporter->saveExportConfiguration();

//        setSettingsModified(true);

//        return true;
//    }

//    return false;
//}

//void PB_StepExportItemList::configureExporterFromModel()
//{
//    // on se place au premier tour (il n'y en a qu'un de toute façon)
//    getInTurnManager()->getTurnIndexManager()->resetTurnIndex();

//    // on demande au gestionnaire de tour de créer la liste des modèles d'entrée pour le premier tour
//    getInTurnManager()->createSearchModelListForCurrentTurn();

//    // on récupère le modèle d'entrée qu'on avait défini
//    CT_InResultModelGroup *resModel = (CT_InResultModelGroup*)getInResultModel(DEFin_res);

//    // on récupère les possibilités pour ce résultat (il n'y en a qu'une puisque un résultat d'entrée = un résultat de sortie)
//    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSavedSelected();

//    QListIterator<CT_InStdModelPossibility*> it(possibilities);

//    QList<CT_AbstractItemDrawable*> eItems;

//    while(it.hasNext())
//    {
//        // on récupère le modèle d'entrée qu'on avait défini (celui à exporter)
//        CT_InAbstractItemModel *inItemModelToExport = NULL;

//        if(_exporter->exportOnlyGroup())
//            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEFin_grp, (CT_OutAbstractResultModel*)it.next()->outModel(), this);
//        else
//            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEFin_item, (CT_OutAbstractResultModel*)it.next()->outModel(), this);

//        // on récupère sa possibilité (il n'y en a qu'une puisqu'on a mis CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple)
//        CT_InStdModelPossibility *possibility = inItemModelToExport->getPossibilitiesSavedSelected().first();

//        // on récupère l'ItemDrawable choisi
//        eItems.append(((CT_OutAbstractItemModel*)possibility->outModel())->itemDrawable());
//    }

//    _exporter->clearErrorMessage();

//    // on les donne à l'exporter
//    _exporter->setItemDrawableToExport(eItems);
//}


// Creation and affiliation of OUT models
void PB_StepExportItemList::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_res = createNewOutResultModel(DEFout_res, tr("Résultat"));
    res_res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExportItemList::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, "", _dir);
    configDialog->addString(tr("Préfixe de nom de fichier"), "", _prefixFileName);

    QStringList list_exportersList;

    QMapIterator<QString, CT_AbstractExporter*> it(_exportersMap);
    while (it.hasNext())
    {
        it.next();
        list_exportersList.append(it.key());
    }

    if (list_exportersList.isEmpty())
    {
        list_exportersList.append(tr("ERREUR : aucun exporter disponible"));
    }

    configDialog->addStringChoice(tr("Choix du type de fichier"), "", list_exportersList, _exportersListValue);

}

void PB_StepExportItemList::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_Item = inResultList.at(0);
    CT_ResultGroup* resIn_Counter = inResultList.at(1);
    CT_ResultGroup* resIn_BaseName = NULL;

    QString rootBaseName = "";
    if (inResultList.size() > 2)
    {
        resIn_BaseName = inResultList.at(2);

        CT_ResultItemIterator it0(resIn_BaseName, this, DEF_inName);
        if (it0.hasNext())
        {
            const CT_AbstractSingularItemDrawable* item = it0.next();
            CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(resIn_BaseName, this, DEF_inNameAtt);
            QString tmp = att->toString(item, NULL);
            rootBaseName = QFileInfo(tmp).baseName();
        }
    }


    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_res = outResultList.at(0);

    QString baseName = "";
    CT_ResultItemIterator it(resIn_Counter, this, DEF_inCounter);
    if (it.hasNext())
    {
        CT_LoopCounter* counter = (CT_LoopCounter*) it.next();
        baseName = counter->getTurnName();
        baseName = QFileInfo(baseName).baseName();
    }

    // IN results browsing
    CT_ResultGroupIterator itIn_grp(resIn_Item, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn = (CT_AbstractItemGroup*) itIn_grp.next();
        CT_AbstractItemDrawable* item = (CT_AbstractItemDrawable*)grpIn->firstItemByINModelName(this, DEFin_item);

        if (item != NULL)
        {
            QList<CT_AbstractItemDrawable*> list;
            list.append(item);

            QString path = "";
            if (_dir.size() > 0)
            {
                path.append(_dir.first());
                path.append("/");
                path.append(rootBaseName);
                path.append(_prefixFileName);
            }

            path.append(baseName);

            CT_AbstractExporter* exporter = _exportersMap.value(_exportersListValue);

            exporter->setExportFilePath(path);

            if (exporter->setItemDrawableToExport(list))
                exporter->exportToFile();
        }

    }
    

    // OUT results creation
    CT_StandardItemGroup* grpOut= new CT_StandardItemGroup(DEFout_grp, res_res);
    res_res->addGroup(grpOut);

}
