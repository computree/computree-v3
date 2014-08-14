#ifndef CT_ITERATORITEMSMODELFORRESULTMODELGROUP_H
#define CT_ITERATORITEMSMODELFORRESULTMODELGROUP_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

class CT_TreeStructureForIterator;
class CT_AbstractItemGroup;

/**
 * @brief Use this iterator if you want to iterate over model that represent singular item in a model
 *        that represent a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_IteratorItemsModelForResultModelGroup
{
public:
    /**
     * @brief Constructor
     * @param group : the model that represent a result where you want iterate
     */
    CT_IteratorItemsModelForResultModelGroup(const CT_OutAbstractResultModelGroup *group);
    ~CT_IteratorItemsModelForResultModelGroup();

    /**
     * @brief initialize the search of the model passed in parameter that represent a singular item
     * @return true if you can use recursiveNextItem()
     */
    bool recursiveBeginIterateItems(const CT_InAbstractSingularItemModel &model);

    /**
     * @brief Returns next model that represent a singular item (that you looking for)
     * @return NULL if the course if completed
     */
    CT_OutAbstractSingularItemModel* recursiveNextItem();

private:

    CT_OutAbstractResultModelGroup              *_resultGroup;

    CT_TreeStructureForIterator                 *_rootTreeModels;
    CT_TreeStructureForIterator                 *_currentTreeModelsChild;

    QList<CT_AbstractItemGroup*>                _groups;

    DEF_CT_AbstractGroupModelOut                *_currentGroup;

    QVector<CT_OutAbstractSingularItemModel*>   _searchItemDrawableModels;
    QList<CT_OutAbstractSingularItemModel*>     _currentGroupItemDrawableModels;

    void initBeginIterate();
    bool continueSearch();

    void clearTreeStructure();
};

#endif // CT_ITERATORITEMSMODELFORRESULTMODELGROUP_H
