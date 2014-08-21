#ifndef CT_RESULTGROUP_H
#define CT_RESULTGROUP_H

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

#include "ct_result/tools/iterator/ct_resultiterator.h"
#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"

#include <QMutex>

/**
 * @brief Represent a result that contains groups
 */
class PLUGINSHAREDSHARED_EXPORT CT_ResultGroup : public CT_AbstractResult
{
public:
    CT_ResultGroup();
    CT_ResultGroup(const CT_OutAbstractResultModelGroup &model,
                   CT_ResultInitializeData &data,
                   CT_VirtualAbstractStep &parentStep);

    ~CT_ResultGroup();

    /**
     * @brief Returns a unique type for a result
     * @overload Overloaded to return a unique type
     */
    QString getType() const;

    /**
     * @brief Returns a unique type for a result
     * @overload Overloaded to return a unique type
     */
    static QString staticGetType();

    /**
     * @brief Add a group to the list of root groups of this result
     * @return Always true to accelerate process. See warning to more information.
     * @warning The method return always true to accelerate process but you must not add group with
     *          another model that the model specified in your step !
     */
    bool addGroup(CT_AbstractItemGroup *group);

    /**
     * @brief Add a list of groups to the list of root groups of this result
     * @return Always true to accelerate process. See warning to more information.
     * @warning The method return always true to accelerate process but you must not add group with
     *          another model that the model specified in your step !
     */
    bool addGroups(const QList<CT_AbstractItemGroup*> &groups);

    /**
     * @brief Return the list of root groups of this result
     */
    const QList<CT_AbstractItemGroup*>& groups() const;


    Q_DECL_DEPRECATED bool setList(QList<CT_AbstractItemGroup*> &list);
    Q_DECL_DEPRECATED const QList<CT_AbstractItemGroup*>& getList() const;


    // Ajout AP 24/07/2013
    Q_DECL_DEPRECATED CT_AbstractItemGroup* beginGroup(const CT_InAbstractGroupModel *model);
    Q_DECL_DEPRECATED CT_AbstractItemGroup* nextGroup();
    Q_DECL_DEPRECATED CT_AbstractSingularItemDrawable* beginItem(const CT_InAbstractSingularItemModel *model);
    Q_DECL_DEPRECATED CT_AbstractSingularItemDrawable* nextItem();

    // Fin d'ajout AP 24/07/2013

    Q_DECL_DEPRECATED bool recursiveBeginIterateItems(const CT_InAbstractSingularItemModel &model);
    Q_DECL_DEPRECATED CT_AbstractSingularItemDrawable* recursiveNextItem();
    Q_DECL_DEPRECATED bool recursiveBeginIterateGroups(const CT_InAbstractGroupModel &model);
    Q_DECL_DEPRECATED CT_AbstractItemGroup* recursiveNextGroup();

    // HERITE DE l'interface Result //
    Q_DECL_DEPRECATED int beginIterateItemDrawableList();
    Q_DECL_DEPRECATED int beginIterateItemDrawableList(bool mutableIterator);
    Q_DECL_DEPRECATED CT_AbstractItemDrawable* nextItemDrawable();

    Q_DECL_DEPRECATED int recursiveBeginIterateItemDrawableWithModel(const CT_AbstractModel &model);
    Q_DECL_DEPRECATED CT_AbstractItemDrawable* recursiveNextItemDrawable();

    /**
     * @brief Use this method to remove an item from this result or something in the structure
     *
     *        Prefer use a MutableIterator to remove an item because is faster than this method.
     *
     * @param parent : the parent of the item or NULL if the item is the root
     * @param item : the item to remove from this result
     * @return true if item is removed
     */
    bool removeItemSomethingInStructure(CT_AbstractItemDrawable *parent,
                                        CT_AbstractItemDrawable *item);

    /**
     * @brief Use this method to remove a group from this result or something in the structure
     *
     *        Prefer use a MutableIterator to remove a group because is faster than this method.
     *
     * @param group : the group to remove from this result
     * @return true if group is removed
     */
    bool removeGroupSomethingInStructure(CT_AbstractItemGroup *group);

    /**
     * @brief Use this method to remove a singular item from this result or something in the structure
     *
     *        Prefer use a MutableIterator to remove an singular item because is faster than this method.
     *
     * @param parent : the parent of the singular item or NULL if the item is the root
     * @param item : the singular item to remove from this result
     * @return true if singular item is removed
     */
    bool removeSingularItemDrawableSomethingInStructure(CT_AbstractItemGroup *parent,
                                                        CT_AbstractSingularItemDrawable *item);

    /**
     * @brief Returns a copy of this model
     */
    CT_AbstractResult* createNewInstance(CT_OutAbstractResultModel *model,
                                         CT_ResultInitializeData &data,
                                         CT_VirtualAbstractStep &parentStep,
                                         CT_ResultCopyModeList copyModeList = (CT_ResultCopyModeList() << CT_ResultCopyModeList::DontCopyItemDrawable));

protected:

    /**
     * @brief When the step finish is compute this method is called.
     * @overload Overload to remove all elements that have been set to be removed later
     */
    void setComplete();

    /**
     * @brief Clear from memory all root groups of this result
     */
    void clearFromMemoryProtected();

private:

    QMutex                                  _mutexAccessItemDrawable;

    QList<CT_AbstractItemGroup*>            _groups;

    CT_ResultIterator                       *m_guiIterator;
    CT_ResultItemIterator                   *m_iIterator;
    CT_ResultGroupIterator                  *m_gIterator;
    CT_ResultGroupIterator                  *m_gRootIterator;

    QList<CT_AbstractItemGroup*>            _currentIterateItemContainer;           /*!< La liste des container qui contiennent les items recherchés lors de l'appel à la méthode beginIterateItems */
    QList<CT_AbstractItemGroup*>            _currentIterateGroupContainer;          /*!< La liste des container qui contiennent les groupes recherchés lors de l'appel à la méthode beginIterateGroups */

    /**
     * @brief Call setBeRemovedLater of all children groups recursively
     */
    void recursiveSetBeRemovedLater(CT_AbstractItemGroup *group);
};

#endif // CT_RESULTGROUP_H
