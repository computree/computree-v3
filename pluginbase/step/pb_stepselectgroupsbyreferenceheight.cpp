#include "pb_stepselectgroupsbyreferenceheight.h"

// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_itemdrawable/ct_referencepoint.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMessageBox>
#include <limits>

// Alias for indexing models
#define DEFin_scres "scres"
#define DEFin_scBase "scBase"
#define DEFin_group "group"
#define DEFin_item "item"
#define DEFin_dtmValue "dtmValue"

// Constructor : initialization of parameters
PB_StepSelectGroupsByReferenceHeight::PB_StepSelectGroupsByReferenceHeight(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _hRef = 1.3;
}

// Step description (tooltip of contextual menu)
QString PB_StepSelectGroupsByReferenceHeight::getStepDescription() const
{
    return tr("Séléction de groupes // hauteur de référence");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSelectGroupsByReferenceHeight::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSelectGroupsByReferenceHeight(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSelectGroupsByReferenceHeight::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_scres = createNewInResultModelForCopy(DEFin_scres, tr("Items"));
    resIn_scres->setZeroOrMoreRootGroup();
    resIn_scres->addGroupModel("", DEFin_scBase, CT_AbstractItemGroup::staticGetType(), tr("Groupe de base"));
    resIn_scres->addItemModel(DEFin_scBase, DEFin_dtmValue, CT_ReferencePoint::staticGetType(), tr("Z MNT"));
    resIn_scres->addGroupModel(DEFin_scBase, DEFin_group, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_scres->addItemModel(DEFin_group, DEFin_item, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
}

// Creation and affiliation of OUT models
void PB_StepSelectGroupsByReferenceHeight::createOutResultModelListProtected()
{
    createNewOutResultModelToCopy(DEFin_scres);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSelectGroupsByReferenceHeight::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addDouble(tr("Hauteur de référence"), "m", 0, 9999, 2, _hRef);

}

void PB_StepSelectGroupsByReferenceHeight::compute()
{
    CT_ResultGroup *resultOut_R = getOutResultList().first();

    QList<CT_AbstractItemGroup*> groupsToRemove;

    CT_ResultGroupIterator itG(resultOut_R, this, DEFin_scBase);
    while(itG.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup *groupOut_G = itG.next();

        if (groupOut_G != NULL)
        {

            CT_ReferencePoint *dtmRefPoint = (CT_ReferencePoint*) groupOut_G->firstItemByINModelName(this, DEFin_dtmValue);

            if (dtmRefPoint != NULL)
            {
                float href = dtmRefPoint->getCenterZ() + _hRef;

                float zmin = std::numeric_limits<float>::max();
                float zmax = -std::numeric_limits<float>::max();

                CT_GroupIterator itG2(groupOut_G, this, DEFin_group);
                while(itG2.hasNext() && !isStopped())
                {

                    const CT_AbstractItemGroup *groupOut_G2 = itG2.next();

                    if (groupOut_G2 != NULL)
                    {
                        CT_AbstractSingularItemDrawable *itemOut_I = groupOut_G2->firstItemByINModelName(this, DEFin_item);

                        if (itemOut_I != NULL)
                        {
                            if (itemOut_I->getCenterZ() < zmin) {zmin = itemOut_I->getCenterZ();}
                            if (itemOut_I->getCenterZ() > zmax) {zmax = itemOut_I->getCenterZ();}
                        }
                    }
                }

                if (zmin > href || zmax < href)
                {
                    groupsToRemove.append((CT_AbstractItemGroup*)groupOut_G);
                }
            }

        }

    }


    // we remove the parent group of all ItemDrawable that must be deleted from the out result
    // and all groups that don't contains a ItemDrawable researched
    QListIterator<CT_AbstractItemGroup*> itE(groupsToRemove);

    while(itE.hasNext())
    {
        CT_AbstractItemGroup *group = itE.next();
        recursiveRemoveGroup(group->parentGroup(), group);
    }

}

void PB_StepSelectGroupsByReferenceHeight::recursiveRemoveGroup(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const
{
    if(parent != NULL)
    {
        parent->removeGroup(group);

        if(parent->isEmptyOfGroups())
            recursiveRemoveGroup(parent->parentGroup(), parent);
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}

