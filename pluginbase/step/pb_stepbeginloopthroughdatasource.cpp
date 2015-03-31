#include "pb_stepbeginloopthroughdatasource.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include "ct_itemdrawable/ct_datasource.h"
#include "ct_global/ct_context.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

#define DEFin_resultItems "resultItems"
#define DEFin_grpItems "grpItems"
#define DEFin_item "item"

#define DEFin_resultDataSource "resultDataSource"
#define DEFin_grpDataSource "grpDataSource"
#define DEFin_dataSource "dataSource"

#define DEFout_result "result"
#define DEFout_grp "grp_PB_StepBeginLoopThroughDataSource"


PB_StepBeginLoopThroughDataSource::PB_StepBeginLoopThroughDataSource(CT_StepInitializeData &dataInit) : CT_StepBeginLoop(dataInit)
{
}

PB_StepBeginLoopThroughDataSource::~PB_StepBeginLoopThroughDataSource()
{
}

QString PB_StepBeginLoopThroughDataSource::getStepDescription() const
{
    return tr("Boucle sur les fichiers d'un DataSource");
}

QString PB_StepBeginLoopThroughDataSource::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* PB_StepBeginLoopThroughDataSource::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepBeginLoopThroughDataSource(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepBeginLoopThroughDataSource::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_dataSource = createNewInResultModel(DEFin_resultDataSource, tr("Source de données"), "", true);
    resIn_dataSource->setZeroOrMoreRootGroup();
    resIn_dataSource->addGroupModel("", DEFin_grpDataSource, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_dataSource->addItemModel(DEFin_grpDataSource, DEFin_dataSource, CT_DataSource::staticGetType(), tr("Source de données"));
}

// Redefine in children steps to complete ConfigurationDialog
void PB_StepBeginLoopThroughDataSource::createPostConfigurationDialog(int &nTurns)
{
    Q_UNUSED(nTurns);
}

// Redefine in children steps to complete out Models
void PB_StepBeginLoopThroughDataSource::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);

    // Récupération du prototype de data_source = la dataSource
    CT_InAbstractResultModel *resultInModel = getInResultModel(DEFin_resultDataSource);

    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractSingularItemModel* sourceModel = NULL;
    CT_DataSource* sourcePrototype = NULL;

    // check if model have choice (can be empty if the step want to create a default out model list)
    if(!resultInModel->getPossibilitiesSavedSelected().isEmpty())
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();

    if(resultInModelOut != NULL)
        sourceModel = (CT_InAbstractSingularItemModel*)PS_MODELS->searchModel(DEFin_dataSource, resultInModelOut, this);

    if((sourceModel != NULL) && !sourceModel->getPossibilitiesSavedSelected().isEmpty())
        sourcePrototype = (CT_DataSource*) ((CT_OutAbstractSingularItemModel*) sourceModel->getPossibilitiesSavedSelected().first()->outModel())->itemDrawable()->copy(NULL, NULL, CT_ResultCopyModeList() << CT_ResultCopyModeList::DontCopyItemDrawable);


    // Création du groupe racine
    CT_OutStdGroupModel *root = new CT_OutStdGroupModel(DEFout_grp);

    if (sourcePrototype != NULL && sourcePrototype->getNumberOfReader() > 0)
    {
        sourcePrototype->init();
        QSharedPointer<CT_AbstractReader> firstReader = sourcePrototype->getNextReader();
        if (!firstReader.isNull())
        {
            _itemModels.clear();
            _groupModels.clear();

            _itemModels.append(firstReader->outItemDrawableModels());
            _groupModels.append(firstReader->outGroupsModel());

            // Ajout du modèle de header
            root->addItem((CT_OutStdSingularItemModel*)firstReader->outHeaderModel()->copy());

            // Ajout des modèles d'items
            QListIterator<CT_OutStdSingularItemModel*> itIM(_itemModels);
            while (itIM.hasNext())
            {
                CT_OutStdSingularItemModel* itemModel = itIM.next();
                root->addItem((CT_OutStdSingularItemModel*)itemModel->copy());
            }

            // Ajout des modèles de groupes
            QListIterator<CT_OutStdGroupModel*> itGM(_groupModels);
            while (itGM.hasNext())
            {
                CT_OutStdGroupModel* groupModel = itGM.next();
                root->addGroup((CT_OutStdGroupModel*)groupModel->copy());
            }
        }
    }

    // Ajout du modèle de résultat
    addOutResultModel(new CT_OutResultModelGroup(DEFout_result, root, tr("Données chargées")));

}

// Redefine in children steps to complete compute method
void PB_StepBeginLoopThroughDataSource::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{   
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_dataSource = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_result = outResultList.at(1);


    CT_ResultItemIterator itIn_dataSource(resIn_dataSource, this, DEFin_dataSource);
    if (itIn_dataSource.hasNext())
    {
        CT_DataSource *dataSource = (CT_DataSource*) itIn_dataSource.next();


        if (dataSource != NULL && dataSource->getNumberOfReader() > 0)
        {

            if (_counter->getCurrentTurn() == 1) {
                _counter->setNTurns(dataSource->getNumberOfReader());
                NTurnsSelected();
            }

            dataSource->init((int)_counter->getCurrentTurn() - 1);
            if (dataSource->activateNextReader())
            {
                QSharedPointer<CT_AbstractReader> reader = dataSource->getActiveReader();

                if (reader->readFile())
                {
                    CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_result);
                    res_result->addGroup(grp_grp);

                    // Ajout du header
                    CT_OutAbstractItemModel *modelCreationH = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(reader->outHeaderModel()->uniqueName(), res_result);
                    CT_AbstractSingularItemDrawable *itemH = reader->takeHeaderCopy(res_result, modelCreationH);
                    grp_grp->addItemDrawable(itemH);

                    // Ajout des items
                    QListIterator<CT_OutStdSingularItemModel*> itIM(_itemModels);
                    while (itIM.hasNext())
                    {
                        CT_OutStdSingularItemModel* itemModel = itIM.next();

                        CT_OutAbstractItemModel *modelCreation = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(itemModel->uniqueName(), res_result);
                        CT_AbstractSingularItemDrawable *item = reader->takeFirstItemDrawableOfModel(itemModel->uniqueName(), res_result, modelCreation);
                        grp_grp->addItemDrawable(item);
                    }

                    // Ajout des groupes
                    QListIterator<CT_OutStdGroupModel*> itGM(_groupModels);
                    while (itGM.hasNext())
                    {
                        CT_OutStdGroupModel* groupModel = itGM.next();

                        CT_OutAbstractItemModel *modelCreation = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(groupModel->uniqueName(), res_result);
                        CT_AbstractItemGroup *groupe = reader->takeFirstGroupOfModel(groupModel->uniqueName(), res_result, modelCreation);
                        grp_grp->addGroup(groupe);
                    }
                }
            }
        }
    }


    setProgress( 100 );
}
