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
    m_readerAddingTools.clear();

    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_res);

    m_readerAddingTools.addReaderResults(DEFin_group, DEFin_reader, res, m_readerAutoIndex);
}

void PB_StepUseReaderToLoadFiles::compute()
{
    CT_ResultGroup *outRes = getOutResultList().first();

    CT_ResultIterator it(outRes, this, DEFin_group);
    m_totalReaderProgress = it.size();
    m_currentReaderProgress = 0;

    if(m_totalReaderProgress > 0) {
        float readerStepProgress = 100.0/m_totalReaderProgress;

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


                    connect(this, SIGNAL(stopped()), reader, SLOT(cancel()), Qt::DirectConnection);
                    connect(reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);

                    //if (reader != NULL && reader->configure() && reader->readFile())
                    if (reader != NULL && reader->readFile())
                        m_readerAddingTools.addReaderResults(outRes, group, reader, m_readerAutoIndex);

                    disconnect(this, NULL, reader, NULL);
                }
            }

            m_currentReaderProgress += readerStepProgress;
        }
    }
}

void PB_StepUseReaderToLoadFiles::readerProgressChanged(int p)
{
    setProgress(m_currentReaderProgress + (p/m_totalReaderProgress));
}
