#include "ct_itemsearchhelper.h"

#include "ct_outabstractresultmodelgroup.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/ct_resultgroup.h"

CT_ItemSearchHelper::CT_ItemSearchHelper()
{
}

CT_ResultItemIterator CT_ItemSearchHelper::searchSingularItemsForModel(const CT_OutAbstractSingularItemModel *model)
{
    if(model != NULL) {
        // get the step of this model
        CT_VirtualAbstractStep *stepOfModel = model->step();

        if(stepOfModel != NULL) {

            // search the result that contains this item (model)
            CT_ResultGroup *theResult = NULL;

            QList<CT_ResultGroup *> results = stepOfModel->getOutResultList();
            QListIterator<CT_ResultGroup*> itR(results);

            while(itR.hasNext() && (theResult == NULL)) {
                CT_ResultGroup *res = itR.next();

                if(res->model()->findModelInTree(model->uniqueNamePlusTurn()))
                    theResult = res;
            }

            if(theResult != NULL)
                return CT_ResultItemIterator(theResult, model);
        }
    }

    return CT_ResultItemIterator();
}
