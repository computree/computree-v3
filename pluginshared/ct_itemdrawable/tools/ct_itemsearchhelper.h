#ifndef CT_ITEMSEARCHHELPER_H
#define CT_ITEMSEARCHHELPER_H

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_iterator/ct_resultitemiterator.h"

/**
 * @brief This class can help you to search an itemdrawable if you have only a (output) model
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemSearchHelper
{
public:
    CT_ItemSearchHelper();

    /**
     * @brief Search items that corresponding to the model passed in parameter and begin the research (recursively) from the step passed in parameter
     * @param model : the model corresponding to the itemdrawable to search
     * @param beginStep : the step where begin the research
     * @return An iterator that can be used to iterate over all items founded
     */
    CT_ResultItemIterator searchSingularItemsForModel(const CT_OutAbstractSingularItemModel *model);
};

#endif // CT_ITEMSEARCHHELPER_H
