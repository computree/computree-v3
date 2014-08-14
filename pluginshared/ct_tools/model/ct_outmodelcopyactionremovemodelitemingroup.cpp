#include "ct_outmodelcopyactionremovemodelitemingroup.h"

#include "ct_virtualabstractstep.h"
#include "ct_outresultmodelgroup.h"
#include "ct_outresultmodelgroupcopy.h"
#include "ct_outresultmodelgrouptocopypossibility.h"

#include "ct_instdgroupmodel.h"

#include "ct_result/model/outModel/tools/ct_iteratoritemsmodelforresultmodelgroup.h"

#include <assert.h>

CT_OutModelCopyActionRemoveModelItemInGroup::CT_OutModelCopyActionRemoveModelItemInGroup(const QString &inItemModelNameToSearchAndRemove) : CT_AbstractOutModelCopyAction()
{
    _inItemModelNameToSearchAndRemove = inItemModelNameToSearchAndRemove;
}

bool CT_OutModelCopyActionRemoveModelItemInGroup::execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy)
{
    // on récupère le résultat modèle de sortie à copier/modifier
    CT_OutAbstractResultModelGroup *outResModel = modelCopy->outModelForModification();

    // on récupère l'item défini dans "createInResultModelListProtected()"
    // pour la recherche
    CT_InAbstractSingularItemModel *inItemModel = (CT_InAbstractSingularItemModel*)step->getInModelForResearch(modelCopy->outModelForSearch(), _inItemModelNameToSearchAndRemove);

    assert(inItemModel != NULL);

    CT_IteratorItemsModelForResultModelGroup it(outResModel);

    // on recherche tous les items du type qui ont été sélectionnés
    // par l'utilisateur
    if(it.recursiveBeginIterateItems(*inItemModel))
    {
        CT_OutAbstractSingularItemModel *outModel;

        while((outModel = it.recursiveNextItem()) != NULL)
        {
            DEF_CT_AbstractGroupModelOut *outModelGroup = dynamic_cast< DEF_CT_AbstractGroupModelOut* >(outModel->parentModel());

            if(!outModelGroup->removeItem(outModel))
                return false;
        }
    }

    return true;
}
