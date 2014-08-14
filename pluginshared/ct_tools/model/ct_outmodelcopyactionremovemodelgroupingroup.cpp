#include "ct_outmodelcopyactionremovemodelgroupingroup.h"

#include "ct_virtualabstractstep.h"
#include "ct_outresultmodelgroup.h"
#include "ct_outresultmodelgroupcopy.h"
#include "ct_outresultmodelgrouptocopypossibility.h"

#include "ct_instdgroupmodel.h"

#include "ct_result/model/outModel/tools/ct_iteratorgroupsmodelforresultmodelgroup.h"

#include <assert.h>

CT_OutModelCopyActionRemoveModelGroupInGroup::CT_OutModelCopyActionRemoveModelGroupInGroup(const QString &inGroupModelNameToSearchAndRemove) : CT_AbstractOutModelCopyAction()
{
    _inGroupModelNameToSearchAndRemove = inGroupModelNameToSearchAndRemove;
}

bool CT_OutModelCopyActionRemoveModelGroupInGroup::execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy)
{
    // on récupère le résultat modèle de sortie à copier/modifier
    CT_OutAbstractResultModelGroup *outResModel = modelCopy->outModelForModification();

    // on récupère le groupe défini dans "createInResultModelListProtected()"
    // pour la recherche
    DEF_CT_AbstractGroupModelIn *inGroupModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(step->getInModelForResearch(modelCopy->outModelForSearch(), _inGroupModelNameToSearchAndRemove));

    assert(inGroupModel != NULL);

    // on recherche tous les groupes du type qui ont été sélectionnés
    // par l'utilisateur
    QList<DEF_CT_AbstractGroupModelOut*> gList;

    CT_IteratorGroupsModelForResultModelGroup it(outResModel);

    if(it.recursiveBeginIterateGroups(inGroupModel))
    {
        DEF_CT_AbstractGroupModelOut *outModel;

        while((outModel = it.recursiveNextGroup()) != NULL)
            gList.append(outModel);
    }

    while(!gList.isEmpty())
    {
        DEF_CT_AbstractGroupModelOut *outModel = gList.takeFirst();
        DEF_CT_AbstractGroupModelOut *parentG = outModel->parentGroup();

        if(parentG != NULL)
        {
            recursiveRemoveAllChildsGroupFromList(outModel, gList); // on supprime tous les fils de ce groupe de la liste

            if(!parentG->removeGroup(outModel))
                return false;
        }
        else
        {
            // si il n'a pas de parent c'est donc le groupe root.
            // on le supprime et on s'arrête puisqu'il n'y aura plus rien en dessous
            outResModel->setRootGroup(NULL);
            return true;
        }
    }

    return true;
}

void CT_OutModelCopyActionRemoveModelGroupInGroup::recursiveRemoveAllChildsGroupFromList(DEF_CT_AbstractGroupModelOut *g, QList<DEF_CT_AbstractGroupModelOut*> &gList)
{
    QListIterator<DEF_CT_AbstractGroupModelOut*> it(g->groups());

    while(it.hasNext())
    {
        recursiveRemoveAllChildsGroupFromList(it.next(), gList);
    }

    gList.removeAll(g);
}
