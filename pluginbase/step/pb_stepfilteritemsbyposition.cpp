#include "pb_stepfilteritemsbyposition.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_beam.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_item "item"



// Constructor : initialization of parameters
PB_StepFilterItemsByPosition::PB_StepFilterItemsByPosition(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _xcoord = 0;
    _ycoord = 0;
    _radius = 20;
}

// Step description (tooltip of contextual menu)
QString PB_StepFilterItemsByPosition::getStepDescription() const
{
    return tr("Garde les items proches d'une coordonnée");
}

// Step detailled description
QString PB_StepFilterItemsByPosition::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepFilterItemsByPosition::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepFilterItemsByPosition::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepFilterItemsByPosition(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepFilterItemsByPosition::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Items à filtrer"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_item, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));

}

// Creation and affiliation of OUT models
void PB_StepFilterItemsByPosition::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepFilterItemsByPosition::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Coordonnée X :", "m", -1e+09, 1e+09, 4, _xcoord, 1);
    configDialog->addDouble("Coordonnée Y :", "m", -1e+09, 1e+09, 4, _ycoord, 1);
    configDialog->addDouble("Rayon", "m", 0, 1e+09, 4, _radius, 1);
}

void PB_StepFilterItemsByPosition::compute()
{

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resCpy_res = outResultList.at(0);

    QList<CT_AbstractItemGroup*> groupsToBeRemoved;

    CT_ResultGroupIterator itCpy_grp(resCpy_res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grpCpy_grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractSingularItemDrawable* itemCpy_item = (CT_AbstractSingularItemDrawable*)grpCpy_grp->firstItemByINModelName(this, DEFin_item);
        if (itemCpy_item != NULL)
        {
            double dist = sqrt(pow(_xcoord - itemCpy_item->getCenterX(), 2) + pow(_ycoord - itemCpy_item->getCenterY(), 2));

            if (dist > _radius)
            {
                groupsToBeRemoved.append(grpCpy_grp);
            }
        }
    }

    while (!groupsToBeRemoved.isEmpty())
    {
        CT_AbstractItemGroup *group = groupsToBeRemoved.takeLast();
        recursiveRemoveGroupIfEmpty(group->parentGroup(), group);
    }
    
}

void PB_StepFilterItemsByPosition::recursiveRemoveGroupIfEmpty(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const
{
    if(parent != NULL)
    {
        parent->removeGroup(group);

        if(parent->isEmpty())
            recursiveRemoveGroupIfEmpty(parent->parentGroup(), parent);
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}

