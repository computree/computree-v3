#include "pb_stepmodifypositions2d.h"
// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"


// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

//Inclusion of actions
#include "actions/pb_actionmodifypositions2d.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMessageBox>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_pos "pos"

#define DEFout_res "res"
#define DEFout_grp "grp"
#define DEFout_pos "pos"


// Constructor : initialization of parameters
PB_StepModifyPositions2D::PB_StepModifyPositions2D(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepModifyPositions2D::getStepDescription() const
{
    return tr("Modifier des positions 2D");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepModifyPositions2D::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepModifyPositions2D(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepModifyPositions2D::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_res = createNewInResultModel(DEFin_res, tr("Positions 2D"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_pos, CT_Point2D::staticGetType(), tr("Position 2D"));
}

// Creation and affiliation of OUT models
void PB_StepModifyPositions2D::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res = createNewOutResultModel(DEFout_res, tr("Positions 2D"));
    res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res->addItemModel(DEFout_grp, DEFout_pos, new CT_Point2D(), tr("Position 2D"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepModifyPositions2D::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepModifyPositions2D::compute()
{
    m_doc = NULL;
    m_status = 0;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* inRes = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    _outRes = outResultList.at(0);

    _modelCreation = (CT_OutAbstractSingularItemModel*)PS_MODELS->searchModelForCreation(DEFout_pos, _outRes);


    // IN results browsing
    CT_ResultGroupIterator itIn_grp(inRes, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grp = (CT_AbstractItemGroup*) itIn_grp.next();

        CT_Point2D* itemIn_pos = (CT_Point2D*)grpIn_grp->firstItemByINModelName(this, DEFin_pos);
        if (itemIn_pos != NULL)
        {
            _positions.append((CT_Point2D*) itemIn_pos->copy(_modelCreation, _outRes, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableCompletely));
        }
    }

    // request the manual mode
    requestManualMode();

    for (int i = 0 ; i < _positions.size() ; i++)
    {
        CT_StandardItemGroup* grp_grp = new CT_StandardItemGroup(DEFout_grp, _outRes);
        _outRes->addGroup(grp_grp);

        grp_grp->addItemDrawable(_positions.at(i));
    }

    m_status = 1;
    requestManualMode();
}



void PB_StepModifyPositions2D::initManualMode()
{
    if(m_doc == NULL)
    {

        QMap<QString, QVariant> param;
        param.insert("Transparency", QVariant(true));

        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument(param);
    }
    m_doc->removeAllItemDrawable();

    m_doc->setCurrentAction(new PB_ActionModifyPositions2D(_positions, _modelCreation, _outRes));

    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette étape"), QMessageBox::Ok);
}

void PB_StepModifyPositions2D::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            m_doc = NULL;
            quitManualMode();
        }
    }
}
