#ifndef CT_RESULTGROUPITERATOR_H
#define CT_RESULTGROUPITERATOR_H

#include "pluginShared_global.h"

#include <QList>

class CT_ResultGroup;
class CT_AbstractItemGroup;

class CT_InAbstractGroupModel;
class CT_OutAbstractGroupModel;

class CT_TreeStructureForIterator;

class CT_VirtualAbstractStep;

/**
 * @brief Class tools to iterate over groups in result
 *
 * @warning Be careful that if groups of the result or child groups is changed (deleted per example) the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_ResultGroupIterator
{
public:
    /**
     * @brief Iterate over all root groups of the result passed in parameter
     */
    CT_ResultGroupIterator(const CT_ResultGroup *result);

    /**
     * @brief Iterate by a IN group model. Recursively search group that correspond to the possibilities of the model passed in parameter and return the next while has next.
     * @param result : the result in which you want to iterate
     * @param inModel : the IN group model that contains all groups to search in the group (parent)
     */
    CT_ResultGroupIterator(const CT_ResultGroup *result, const CT_InAbstractGroupModel *inModel);

    /**
     * @brief Iterate by a OUT group model. Recursively search group that correspond to the model passed in parameter and return the next while has next.
     * @param result : the result in which you want to iterate
     * @param outModel : the OUT group model that contains all groups to search in the group (parent)
     */
    CT_ResultGroupIterator(const CT_ResultGroup *result, const CT_OutAbstractGroupModel *outModel);

    /**
     * @brief Iterate by a IN or OUT group model name.
     * @param result : the result in which you want to iterate
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     * @param modelName : the IN or OUT group model name to search (typically a DEF_...)
     */
    CT_ResultGroupIterator(const CT_ResultGroup *result, const CT_VirtualAbstractStep *step, const QString &modelName);

    ~CT_ResultGroupIterator();

    /**
     * @brief Returns true if there is at least one group ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    const CT_AbstractItemGroup* next();

private:
    CT_ResultGroup                          *m_result;

    CT_TreeStructureForIterator             *m_rootTree;
    mutable CT_TreeStructureForIterator     *m_currentInTree;

    mutable CT_AbstractItemGroup            *m_current;
    mutable bool                            m_hasNextCalled;

    void initIterator(const CT_ResultGroup *result, const CT_InAbstractGroupModel *inModel);
    void initIterator(const CT_ResultGroup *result, const CT_OutAbstractGroupModel *outModel);

    template<class IT>
    bool hasNextT(IT *it) const;

    bool hasNextWithCurrentChild() const;

    void recursiveDeleteIterator(CT_TreeStructureForIterator *tsfi);
};

#endif // CT_RESULTGROUPITERATOR_H
