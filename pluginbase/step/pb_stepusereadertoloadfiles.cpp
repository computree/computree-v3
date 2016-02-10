#include "pb_stepusereadertoloadfiles.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_view/ct_stepconfigurabledialog.h"


#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_group "group"
#define DEFin_reader "reader"
#define DEFin_conditionnalItem "conditem"
#define DEFin_conditionnalAttribute "condatt"

// Constructor : initialization of parameters
PB_StepUseReaderToLoadFiles::PB_StepUseReaderToLoadFiles(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _conditionnal = false;
}

PB_StepUseReaderToLoadFiles::~PB_StepUseReaderToLoadFiles()
{
}

// Step description (tooltip of contextual menu)
QString PB_StepUseReaderToLoadFiles::getStepDescription() const
{
    return tr("Charger des fichiers à l'aide de readers");
}

// Step detailled description
QString PB_StepUseReaderToLoadFiles::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepUseReaderToLoadFiles::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepUseReaderToLoadFiles::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepUseReaderToLoadFiles(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepUseReaderToLoadFiles::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();
    configDialog->addBool(tr("Conditionner le chargement à un attribut booléen"), "", "", _conditionnal);
}


// Creation and affiliation of IN models
void PB_StepUseReaderToLoadFiles::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *res = createNewInResultModelForCopy(DEFin_res, tr("Fichiers"));
    res->setZeroOrMoreRootGroup();
    res->addGroupModel("", DEFin_group);
    res->addItemModel(DEFin_group, DEFin_reader, CT_ReaderItem::staticGetType(), tr("Reader"));
    if (_conditionnal)
    {
        res->addItemModel(DEFin_group, DEFin_conditionnalItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item conditionnant le chargement"));
        res->addItemAttributeModel(DEFin_conditionnalItem, DEFin_conditionnalAttribute, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::BOOLEAN, tr("Attribut conditionnel"));
    }
}


// Creation and affiliation of OUT models
void PB_StepUseReaderToLoadFiles::createOutResultModelListProtected()
{
    _itemModels.clear();

    // Récupération du prototype de ct_readeritem
    CT_InAbstractResultModel *resultInModel = getInResultModel(DEFin_res);

    if (resultInModel != NULL)
    {
        CT_OutAbstractResultModel* resultInModelOut = NULL;
        CT_InAbstractSingularItemModel* readerItemModel = NULL;
        CT_ReaderItem* readerItemPrototype = NULL;

        // check if model have choice (can be empty if the step want to create a default out model list)
        if(!resultInModel->getPossibilitiesSavedSelected().isEmpty())
            resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();

        if(resultInModelOut != NULL)
            readerItemModel = (CT_InAbstractSingularItemModel*)PS_MODELS->searchModel(DEFin_reader, resultInModelOut, this);

        if((readerItemModel != NULL) && !readerItemModel->getPossibilitiesSavedSelected().isEmpty())
            readerItemPrototype = (CT_ReaderItem*) ((CT_OutAbstractSingularItemModel*) readerItemModel->getPossibilitiesSavedSelected().first()->outModel())->itemDrawable();

        if (readerItemPrototype != NULL)
        {

            CT_AbstractReader* reader = readerItemPrototype->getReader();
            reader->init();

            CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_res);
            QList<CT_OutStdSingularItemModel*> itemModels = reader->outItemDrawableModels();

            if (res != NULL)
            {
                // Ajout du modèle de header
                _headerModel = new CT_AutoRenameModels();
                res->addItemModel(DEFin_group, *_headerModel, new CT_FileHeader(), reader->getHeader()->displayableName());

                // Ajout des modèles d'items
                QListIterator<CT_OutStdSingularItemModel*> itIM(itemModels);
                while (itIM.hasNext())
                {
                    CT_OutStdSingularItemModel* itemModel = itIM.next();
                    CT_AutoRenameModels* autoRename = new CT_AutoRenameModels();

                    _itemModels.insert(itemModel->uniqueName(), autoRename);
                    res->addItemModel(DEFin_group, *autoRename, (CT_AbstractSingularItemDrawable*) (itemModel->itemDrawable()->copy(NULL, NULL, CT_ResultCopyModeList())), itemModel->displayableName());
                }
            }
        }
    }
}


void PB_StepUseReaderToLoadFiles::compute()
{

    CT_ResultGroup *outRes = getOutResultList().first();

    CT_ResultGroupIterator it(outRes, this, DEFin_group);
    while (it.hasNext() && (!isStopped()))
    {
        CT_StandardItemGroup *group = (CT_StandardItemGroup*) it.next();
        CT_ReaderItem *readerItem = (CT_ReaderItem*)group->firstItemByINModelName(this, DEFin_reader);

        if(readerItem != NULL)
        {
            bool load = true;
            if (_conditionnal)
            {
                CT_AbstractSingularItemDrawable *conditionnalItem = (CT_AbstractSingularItemDrawable*)group->firstItemByINModelName(this, DEFin_conditionnalItem);
                if (conditionnalItem != NULL)
                {
                    bool attributeValue = conditionnalItem->firstItemAttributeByINModelName(outRes, this, DEFin_conditionnalAttribute)->toBool(conditionnalItem, NULL);
                    load = attributeValue;
                } else {
                    load = false;
                }
            }

            if (load)
            {
                CT_AbstractReader* reader = readerItem->getReader();
                if (reader != NULL && reader->readFile())
                {
                    QListIterator<CT_OutStdSingularItemModel*> it(reader->outItemDrawableModels());

                    while(it.hasNext())
                    {
                        CT_OutStdSingularItemModel *model = it.next();
                        CT_AutoRenameModels *modelCreationAutoRename = _itemModels.value(model->uniqueName());

                        if (modelCreationAutoRename != NULL)
                        {
                            QList<CT_AbstractSingularItemDrawable*> items = reader->takeItemDrawableOfModel(model->uniqueName(), outRes, modelCreationAutoRename->completeName());
                            QListIterator<CT_AbstractSingularItemDrawable*> itI(items);

                            while(itI.hasNext())
                            {
                                CT_AbstractSingularItemDrawable* item = itI.next();
                                group->addItemDrawable(item);
                            }
                        }
                    }

                    CT_FileHeader* header = reader->takeHeaderCopy(outRes, _headerModel->completeName());
                    if (header != NULL) {group->addItemDrawable(header);}
                }
            }
        }
    }
}
