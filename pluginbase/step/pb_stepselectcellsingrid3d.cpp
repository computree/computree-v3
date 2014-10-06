#include "pb_stepselectcellsingrid3d.h"

// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_itemdrawable/ct_grid3d.h"

//Inclusion of actions
#include "actions/pb_actionselectcellsingrid3d.h"
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMessageBox>

// Alias for indexing in models
#define DEF_resultIn "result"
#define DEF_groupIn "group"
#define DEF_itemBaseGrid "basegrid"

#define DEF_resultOut "result"
#define DEF_outGroup "group"
#define DEF_outBoolGrid "boolGrid"
#define DEF_outFilteredGrid "filteredGrid"

// Constructor : initialization of parameters
PB_StepSelectCellsInGrid3D::PB_StepSelectCellsInGrid3D(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    _refGrid = NULL;
    _boolGrid = NULL;
    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepSelectCellsInGrid3D::getStepDescription() const
{
    return tr("Séléction d'un sous-ensemble d'une grille 3D");
}

// Step description (tooltip of contextual menu)
QString PB_StepSelectCellsInGrid3D::getStepDetailledDescription() const
{
    return tr("Cette étape permet de générer une grille booléenne, représentant une séléction de cellules parmi celles de la grille de référence choisie en entrée.<br>"
              "Elle utilise un actionner, permettant de faire des séléction soit par plans horizontaux 2D, soit directement en 3D.<br>"
              "En sortie elle fournie également une copie de la grille d'entrée pour laquelle toute les cases non sélectionnées sont réinitialisées à la valeur 0. ");
}

QString PB_StepSelectCellsInGrid3D::getStepURL() const
{
    return tr("http://rdinnovation.onf.fr/projects/plugin-base/wiki/Fr_action_selectcells");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSelectCellsInGrid3D::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSelectCellsInGrid3D(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSelectCellsInGrid3D::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultInModel = createNewInResultModel(DEF_resultIn,
                                                                  tr("Result"),
                                                                  tr(""),
                                                                  false);
    resultInModel->setZeroOrMoreRootGroup();
    resultInModel->addGroupModel("", DEF_groupIn);
    resultInModel->addItemModel(DEF_groupIn,
                                DEF_itemBaseGrid,
                                CT_AbstractGrid3D::staticGetType(),
                                tr("Grille"));
}

// Creation and affiliation of OUT models
void PB_StepSelectCellsInGrid3D::createOutResultModelListProtected()
{
    CT_InAbstractResultModel *resultInModel = getInResultModel(DEF_resultIn);

    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractSingularItemModel* baseGridModel = NULL;
    CT_AbstractSingularItemDrawable* itemPrototype = NULL;

    // check if model have choice (can be empty if the step want to create a default out model list)
    if(!resultInModel->getPossibilitiesSavedSelected().isEmpty())
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();

    if(resultInModelOut != NULL)
        baseGridModel = (CT_InAbstractSingularItemModel*)PS_MODELS->searchModel(DEF_itemBaseGrid, resultInModelOut, this);

    if((baseGridModel != NULL) && !baseGridModel->getPossibilitiesSavedSelected().isEmpty())
        itemPrototype = (CT_AbstractSingularItemDrawable*) ((CT_OutAbstractSingularItemModel*) baseGridModel->getPossibilitiesSavedSelected().first()->outModel())->itemDrawable()->copy(NULL, NULL, CT_ResultCopyModeList() << CT_ResultCopyModeList::DontCopyItemDrawable);

    // if we don't have a possibility and we don't create a default out model, there was a problem
    if((itemPrototype == NULL) && !isCreateDefaultOutModelActive())
        qFatal("Error creating out model in PB_StepSelectCellsInGrid3D");

    // create the out result
    CT_OutResultModelGroup *resultModel = createNewOutResultModel(DEF_resultOut,
                                                                  tr("Grille filtrée"),
                                                                  tr("Grille filtrée"));
    resultModel->setRootGroup(DEF_outGroup);
    resultModel->addItemModel(DEF_outGroup, DEF_outBoolGrid, new CT_Grid3D<bool>(), tr("Cases séléctionnées"));
    resultModel->addItemModel(DEF_outGroup, DEF_outFilteredGrid, itemPrototype, tr("Grille filtrée"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSelectCellsInGrid3D::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepSelectCellsInGrid3D::compute()
{
    m_doc = NULL;
    m_status = 0;

    CT_ResultGroup *resultIn = getInputResults().first();

    CT_ResultGroup *resultOut = getOutResultList().first();
    CT_OutStdSingularItemModel* filteredGridModel = (CT_OutStdSingularItemModel*)PS_MODELS->searchModelForCreation(DEF_outFilteredGrid, resultOut);

    // create a list of itemdrawable to add in the document
    CT_ResultGroupIterator itG(resultIn, this, DEF_groupIn);

    if (itG.hasNext())
    {
        const CT_AbstractItemGroup *groupIn = itG.next();

        CT_AbstractGrid3D *baseGrid = (CT_AbstractGrid3D*) groupIn->firstItemByINModelName(this, DEF_itemBaseGrid);
        if (baseGrid!=NULL)
        {
            _boolGrid = new CT_Grid3D<bool>(DEF_outBoolGrid, resultOut,
                                            baseGrid->minX(), baseGrid->minY(), baseGrid->minZ(),
                                            baseGrid->xdim(), baseGrid->ydim(), baseGrid->zdim(),
                                            baseGrid->resolution(), false, false);
            _refGrid = baseGrid;

            CT_AbstractGrid3D *filteredGrid = (CT_AbstractGrid3D*) baseGrid->copy(filteredGridModel, resultOut, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableCompletely);

            // request the manual mode
            requestManualMode();

            m_status = 1;
            requestManualMode();

            // Filter output grid, copied from reference grid
            for (size_t i = 0 ; i < _refGrid->nCells() ; i++)
            {
                if (!_boolGrid->valueAtIndex(i))
                {
                    filteredGrid->setValueAtIndexFromDouble(i, 0);
                }
            }

            CT_StandardItemGroup* outGroup = new CT_StandardItemGroup(DEF_outGroup, resultOut);
            outGroup->addItemDrawable(_boolGrid);
            outGroup->addItemDrawable(filteredGrid);
            resultOut->addGroup(outGroup);
        }
    }

}

void PB_StepSelectCellsInGrid3D::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();

        PB_ActionSelectCellsInGrid3D *action = new PB_ActionSelectCellsInGrid3D(_refGrid, _boolGrid);

        // check if don't exist in the action manager
        if(!getGuiContext()->actionsManager()->existAction(action->uniqueName()))
        {
            // if not we add it to the manager. The action is deleted by the manager.
            getGuiContext()->actionsManager()->addAction(action);
        }

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        m_doc->setCurrentAction(action);
    }

    m_doc->removeAllItemDrawable();

    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette étape de filtrage.\n"
                                                         "Seuls les voxels séléctionés (Rouges) seront conservés.\n"
                                                         "Laisser la souris au-dessus d'un bouton pour avoir des infos."), QMessageBox::Ok);
}

void PB_StepSelectCellsInGrid3D::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            // nothing to do, _boolGrid has already been modified in manual mode
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            getGuiContext()->documentManager()->closeDocument(m_doc);
            m_doc = NULL;

            quitManualMode();
        }
    }
}
