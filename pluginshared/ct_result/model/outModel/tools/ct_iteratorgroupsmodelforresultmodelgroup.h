#ifndef CT_ITERATORGROUPSMODELFORRESULTMODELGROUP_H
#define CT_ITERATORGROUPSMODELFORRESULTMODELGROUP_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_itemdrawable/model/inModel/abstract/def_ct_abstractgroupmodelin.h"

class CT_TreeStructureForIterator;
class CT_AbstractItemGroup;

/**
 * @brief Use this iterator if you want to iterate over model that represent group in a model
 *        that represent a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_IteratorGroupsModelForResultModelGroup
{
public:
    /**
     * @brief Constructor
     * @param group : the model that represent a result where you want iterate
     */
    CT_IteratorGroupsModelForResultModelGroup(const CT_OutAbstractResultModelGroup *resultModel);
    ~CT_IteratorGroupsModelForResultModelGroup();

    /**
     * @brief initialize the search of the model passed in parameter that represent a group
     * @return true if you can use recursiveNextGroup()
     */
    bool recursiveBeginIterateGroups(const DEF_CT_AbstractGroupModelIn *model);

    /**
     * @brief Returns next model that represent a group (that you looking for)
     * @return NULL if the course if completed
     */
    DEF_CT_AbstractGroupModelOut* recursiveNextGroup();

private:

    CT_OutAbstractResultModelGroup              *_resultGroup;

    CT_TreeStructureForIterator                 *_rootTreeModels;
    CT_TreeStructureForIterator                 *_currentTreeModelsChild;

    QList<CT_AbstractItemGroup*>                _groups;

    DEF_CT_AbstractGroupModelOut                *_currentGroup;

    void initBeginIterate();
    bool continueSearch();

    void clearTreeStructure();
};

#endif // CT_ITERATORGROUPSMODELFORRESULTMODELGROUP_H
