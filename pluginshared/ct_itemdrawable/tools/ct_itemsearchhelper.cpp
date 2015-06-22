#include "ct_itemsearchhelper.h"

#include "ct_outabstractresultmodelgroup.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_ItemSearchHelper::CT_ItemSearchHelper()
{
}

CT_ResultItemIterator CT_ItemSearchHelper::searchSingularItemsForModel(const CT_OutAbstractSingularItemModel *model)
{
    if(model != NULL) {
        // get the step of this model
        CT_VirtualAbstractStep *stepOfModel = model->step();

        if(stepOfModel != NULL) {

            // search the model of the result
            CT_AbstractModel *m = model->parentModel();
            CT_OutAbstractResultModelGroup *mg = NULL;

            while((m != NULL) && ((mg = dynamic_cast<CT_OutAbstractResultModelGroup*>(m)) == NULL))
                m = m->parentModel();

            if(mg != NULL) {

                // get the result corresponding to his model
                CT_ResultGroup *result = stepOfModel->getOutputResultForModel(mg);

                if(result != NULL)
                    return CT_ResultItemIterator(result, model);
            }
        }
    }

    return CT_ResultItemIterator();
}
