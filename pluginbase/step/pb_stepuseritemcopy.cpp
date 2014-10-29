#include "pb_stepuseritemcopy.h"

// Inclusion of in models
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"

// Inclusion of out models
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_cylinder.h"

//Inclusion of actions
#include "actions/pb_actionselectitemdrawablegv.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMessageBox>

// Alias for indexing in models
#define DEF_resultIn_R "Ri"
#define DEF_groupIn_G "Gi"
#define DEF_itemIn_I "Ii"

#define DEF_resultOut_R "Ro"
#define DEF_groupOut_G "Go"
#define DEF_itemOut_I "Io"


// Constructor : initialization of parameters
PB_StepUserItemCopy::PB_StepUserItemCopy(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepUserItemCopy::getStepDescription() const
{
    return tr("Séléction d'un sous-ensemble d'items (copie les items)");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepUserItemCopy::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepUserItemCopy(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepUserItemCopy::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultInModel_R = createNewInResultModel(DEF_resultIn_R,
                                                                    tr("Result"));
    resultInModel_R->setZeroOrMoreRootGroup();
    resultInModel_R->addGroupModel("", DEF_groupIn_G);
    resultInModel_R->addItemModel(DEF_groupIn_G,
                                  DEF_itemIn_I,
                                  CT_Cylinder::staticGetType(),
                                  tr("Cylinder"));
}

// Creation and affiliation of OUT models
void PB_StepUserItemCopy::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *resultOutModel_R = createNewOutResultModel(DEF_resultOut_R,
                                                                       tr("Result"));
    resultOutModel_R->setRootGroup(DEF_groupOut_G);
    resultOutModel_R->addItemModel(DEF_groupOut_G,
                                   DEF_itemOut_I,
                                   new CT_Cylinder(),
                                   tr("Cylinder"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepUserItemCopy::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepUserItemCopy::compute()
{
    m_doc = NULL;
    m_status = 0;

    CT_ResultGroup *inRes = getInputResults().first();
    CT_InAbstractGroupModel* groupInModel_G = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_groupIn_G, inRes, this);

    m_itemDrawableToAdd.clear();

    QList<CT_AbstractItemGroup*> emptyGroups;

    CT_ResultGroupIterator itR(inRes, groupInModel_G);

    // create a list of itemdrawable to add in the document
    while(itR.hasNext()
          && !isStopped())
    {
        const CT_AbstractItemGroup *group = itR.next();
        CT_Cylinder *itemOut_I = (CT_Cylinder*) group->firstItemByINModelName(this, DEF_itemIn_I);

        if(itemOut_I != NULL)
            m_itemDrawableToAdd.insert(itemOut_I, (CT_AbstractItemGroup*)group);
        else
            emptyGroups.append((CT_AbstractItemGroup*)group);
    }

    // request the manual mode
    requestManualMode();


    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup *resultOut_R = outResultList.first();
    CT_OutAbstractSingularItemModel* itemOutModel_I = (CT_OutAbstractSingularItemModel*)PS_MODELS->searchModelForCreation(DEF_itemOut_I, resultOut_R);

    QListIterator<CT_AbstractItemDrawable*> it(m_itemDrawableSelected);
    while (it.hasNext())
    {
        CT_Cylinder *cylinder = dynamic_cast<CT_Cylinder*>(it.next());

        if (cylinder != NULL)
        {
            CT_Cylinder *cylinderCopy = (CT_Cylinder*) cylinder->copy(itemOutModel_I, resultOut_R, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableCompletely);
            CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_groupOut_G, resultOut_R);
            group->addItemDrawable(cylinderCopy);
            resultOut_R->addGroup(group);
        }
    }

    m_status = 1;
    requestManualMode();
}

void PB_StepUserItemCopy::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();

        PB_ActionSelectItemDrawableGV *action = new PB_ActionSelectItemDrawableGV();

        // check if don't exist in the action manager
        if(!getGuiContext()->actionsManager()->existAction(action->uniqueName()))
        {
            // if not we add it to the manager. The action is deleted by the manager.
            getGuiContext()->actionsManager()->addAction(action);
        }

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        m_doc->setCurrentAction(action);
    }

    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    // TODO add async with GuiManagerInterface
    QHashIterator<CT_AbstractSingularItemDrawable*, CT_AbstractItemGroup*> it(m_itemDrawableToAdd);

    while(it.hasNext())
        m_doc->addItemDrawable(*it.next().key());

    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette "
                                                         "étape de filtrage. Veuillez sélectionner les "
                                                         "éléments dans la vue graphique puis valider en cliquant "
                                                         "sur le pouce en haut de la fenêtre principale. Les éléments "
                                                         "sélectionnés seront gardés dans le résultat de sortie."), QMessageBox::Ok);
}

void PB_StepUserItemCopy::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            m_itemDrawableSelected = m_doc->getSelectedItemDrawable();
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
