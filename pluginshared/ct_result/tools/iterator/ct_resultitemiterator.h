#ifndef CT_RESULTITEMITERATOR_H
#define CT_RESULTITEMITERATOR_H

#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

#include <QList>

class CT_ResultGroup;
class CT_AbstractSingularItemDrawable;

class CT_InAbstractSingularItemModel;

class CT_TreeStructureConstructor;
class CT_TreeStructureForIterator;

/**
 * @brief Class tools to iterate over itemdrawable in result
 *
 * @warning Be careful that if groups of the result or itemdrawable is changed (deleted per example) the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_ResultItemIterator
{
public:
    /**
     * @brief Recursively iterate over all itemdrawable of the result passed in parameter
     */
    CT_ResultItemIterator(const CT_ResultGroup *result);

    /**
     * @brief Iterate by a IN itemdrawable model. Recursively search items that correspond to the possibilities of the model passed in parameter and return the next while has next.
     * @param result : the result in which you want to iterate
     * @param inModel : the IN itemdrawable model that contains all items to search in groups (parents)
     */
    CT_ResultItemIterator(const CT_ResultGroup *result, const CT_InAbstractSingularItemModel *inModel);

    /**
     * @brief Iterate by a OUT itemdrawable model. Recursively search items that correspond to the model passed in parameter and return the next while has next.
     * @param result : the result in which you want to iterate
     * @param outModel : the OUT itemdrawable model that contains all items to search in groups (parents)
     */
    CT_ResultItemIterator(const CT_ResultGroup *result, const CT_OutAbstractSingularItemModel *outModel);

    /**
     * @brief Iterate by a IN or OUT itemdrawable model name.
     * @param result : the result in which you want to iterate
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     * @param modelName : the IN or OUT itemdrawable model name to search (typically a DEF_...)
     */
    CT_ResultItemIterator(const CT_ResultGroup *result, const CT_VirtualAbstractStep *step, const QString &modelName);

    ~CT_ResultItemIterator();

    /**
     * @brief Returns true if there is at least one item ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    const CT_AbstractSingularItemDrawable* next();

private:
    CT_ResultGroup                          *m_result;

    CT_TreeStructureForIterator             *m_rootTree;
    mutable CT_TreeStructureForIterator     *m_currentInTree;

    mutable CT_AbstractSingularItemDrawable *m_current;
    mutable bool                            m_hasNextCalled;

    void initIterator(const CT_ResultGroup *result, const CT_InAbstractSingularItemModel *inModel);
    void initIterator(const CT_ResultGroup *result, const CT_OutAbstractSingularItemModel *outModel);

    template<class IT>
    bool hasNextT(IT *it) const;

    bool hasNextWithCurrentChild() const;

    void recursiveCreateStructureForAllItems(DEF_CT_AbstractGroupModelOut *groupModel, CT_TreeStructureConstructor *constructor);
    void recursiveDeleteIterator(CT_TreeStructureForIterator *tsfi);
};

#endif // CT_RESULTITEMITERATOR_H
