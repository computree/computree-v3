#ifndef CT_RESULTITERATOR_H
#define CT_RESULTITERATOR_H

#include "pluginShared_global.h"

#include <QList>

class CT_ResultGroup;
class CT_ResultItemIterator;
class CT_ResultGroupIterator;
class CT_AbstractModel;
class CT_AbstractItem;

/**
 * @brief Class tools to iterate over element in result
 *
 * @warning Be careful that if elements of the result is changed (deleted per example) the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_ResultIterator
{
public:
    /**
     * @brief Create a iterator for the result passed in parameter.
     * @param result : the result to iterate
     * @param recursiveIteration : true if you want to iterate over all childrens (recursively). False if you want to iterate over root
     *        items of the result
     */
    CT_ResultIterator(const CT_ResultGroup *result,
                      bool recursiveIteration);

    /**
     * @brief Create a iterator for the result passed in parameter. It iterate over childrens (recursively) defined by the model passed
     *        in parameter.
     * @param result : the result to iterate
     * @param model : the model searched
     */
    CT_ResultIterator(const CT_ResultGroup *result,
                      const CT_AbstractModel *model);
    ~CT_ResultIterator();

    /**
     * @brief Return how many groups or items you must iterate.
     *
     * If you call this method this object must iterate a first time
     * over all elements searched for get the size and a second time when you call next (slow)
     */
    int size() const;

    /**
     * @brief Return true if you can call "next()" method
     */
    bool hasNext() const;

    /**
     * @brief Return the next element
     */
    const CT_AbstractItem* next();

private:
    CT_ResultGroup                              *m_result;
    CT_AbstractModel                            *m_model;
    mutable CT_ResultItemIterator               *m_itemIT;
    mutable CT_ResultGroupIterator              *m_groupIT;
    mutable QList<CT_AbstractItem*>             m_collection;
    mutable int                                 m_size;

    template<typename T>
    void initGroupT(const CT_AbstractModel *model);

    template<typename T>
    void initItemT(const CT_AbstractModel *model);
};

#endif // CT_RESULTITERATOR_H
