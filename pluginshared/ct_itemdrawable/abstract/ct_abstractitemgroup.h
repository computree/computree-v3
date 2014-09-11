#ifndef CT_ABSTRACTITEMGROUP_H
#define CT_ABSTRACTITEMGROUP_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"

class CT_OutAbstractGroupModel;
class CT_OutAbstractSingularItemModel;

class CT_ResultGroup;
class CT_Container;

/**
 * @brief Tool class to stock an item and other information. Used by a group to now if this element must be removed later.
 * @warning Not intended for direct use by plugin developper
 */
class PLUGINSHAREDSHARED_EXPORT CT_GroupItemDrawableContainer
{
public:
    CT_GroupItemDrawableContainer(CT_AbstractSingularItemDrawable *item) : m_item(item), m_willBeRemovedLater(false), m_autoDelete(true) {}
    ~CT_GroupItemDrawableContainer() { if(m_autoDelete) { delete m_item; } }

    inline void setWillBeRemovedLater() { m_willBeRemovedLater = true; }
    inline bool willBeRemovedLater() const { return m_willBeRemovedLater; }

    inline CT_AbstractSingularItemDrawable* item() const { return m_item; }

    CT_GroupItemDrawableContainer* copy()
    {
        CT_GroupItemDrawableContainer *ct = new CT_GroupItemDrawableContainer(m_item);
        ct->m_autoDelete = false;

        return ct;
    }

private:
    CT_AbstractSingularItemDrawable *m_item;
    bool                            m_willBeRemovedLater;
    bool                            m_autoDelete;
};

/**
 * @brief Represent a group that can contains other groups or other singular item (leaf). Its a node in a tree structure of items.
 *
 * The advantage is that you can process groups in multithread. Per example if you want to convert all CT_PointCluster contained in groups to
 * a CT_Circle you can process each group with a different thread.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemGroup : public CT_AbstractItemDrawable
{
    Q_OBJECT

public:

    /*!
     *  \brief Emplacement de recherche des groupes et items
     *
     *  Si vous ne savez pas lequel utilisé : mettre (S_New | S_Backup)
     */
    enum SMode
    {
        S_New = 1,      /*!< Recherche dans la liste des éléments venant d'être insérés */
        S_Backup = 2    /*!< Recherche dans la liste des éléments sauvegardés (les éléments nouvellement insérés sont sauvegardés lorsque le résultat ou le groupe a été copié) */
    };
    Q_DECLARE_FLAGS(SearchMode, SMode)

    enum RemoveLaterBit
    {
        Nothing = 0,
        RemoveLater = 1,
        ChildRemoveLater = 2
    };
    Q_DECLARE_FLAGS(RemoveLaterBits, RemoveLaterBit)

    CT_AbstractItemGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractItemGroup(const CT_OutAbstractGroupModel *model,
                         const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItemGroup(const QString &modelName,
                         const CT_AbstractResult *result);

    ~CT_AbstractItemGroup();

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous héritez de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    /**
     * @brief Change the result that contains this item.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void changeResult(const CT_AbstractResult *newRes);

    /**
     * @brief Returns true if the groups conatins no item and no groups (test flags willBeRemovedLater() to know the real number)
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Returns childrens of this group for the GUI
     */
    QList<CT_AbstractItem*> childrensForGui() const;

    /****** SINGULAR ITEMDRAWABLE METHODS ********/

    /**
     * @brief Add a ItemDrawable to the group.
     *
     *        The model of the ItemDrawable must not be NULL. The model will be tested and if this group contains already a ItemDrawable with
     *        the same model the ItemDrawable will not be added and the method return false.
     *
     * @return false if a ItemDrawable with the same model (compare uniqueName()) already exist in this group.
     */
    virtual bool addItemDrawable(CT_AbstractSingularItemDrawable *item) = 0;

    /**
     * @brief Remove the item from the group. The item is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the item must be removed. This is a choice to help users who use multithreaded
     *        not worry about problems of access to items removed from memory.
     *
     *        All methods that return the number of ItemDrawable in this group will return the number of ItemDrawable that has not be
     *        removed (the flag is not set).
     */
    virtual bool removeItem(CT_AbstractSingularItemDrawable *item) = 0;

    /**
     * @brief Returns true if this group contains a ItemDrawable that match with at least one of the possibility of the IN model.
     */
    virtual bool containsItemDrawable(const CT_InAbstractSingularItemModel *model) const = 0;

    /**
     * @brief Returns true if the group contains a ItemDrawable that match with the OUT model
     */
    virtual bool containsItemDrawable(const CT_OutAbstractSingularItemModel *model) const = 0;

    /**
     * @brief Returns true if the group contains a ItemDrawable that match with the IN or OUT model to search by modelName
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     * @param modelName : typically a DEF_... in your step
     */
    bool containsItemDrawableByModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /**
     * @brief How many ItemDrawable ? This method takes into account the deleted item (item that flag removed is set, show method removeItem(...) for more information)
     */
    virtual int nItems() const = 0;

    /**
     * @brief Returns true if the group don't contains singular ItemDrawable
     */
    virtual bool isEmptyOfSingularItems() const = 0;

    /**
     * @brief Returns a hashMap of CT_AbstractSingularItemDrawable. The key is the modelName.
     */
    virtual QHash<QString, CT_GroupItemDrawableContainer*> items() const = 0;

    /**
     * @brief Returns a hashMap of new added CT_AbstractSingularItemDrawable. The key is the modelName.
     */
    virtual const QHash<QString, CT_GroupItemDrawableContainer*>& itemsNew() const = 0;

    /**
     * @brief Returns a hashMap of old CT_AbstractSingularItemDrawable (items that have been added to the group if it was a copy). The key is the modelName.
     */
    virtual const QHash<QString, CT_GroupItemDrawableContainer*>& itemsBackup() const = 0;

    /**
     * @brief Returns the ItemDrawable corresponding to the OUT model
     *
     * @return NULL if no item will be found
     */
    virtual CT_AbstractSingularItemDrawable* item(const CT_OutAbstractSingularItemModel *model) const = 0;

    /**
     * @brief Returns the ItemDrawable corresponding to the OUT model to search by modelName.
     *
     * @param modelName : typically a DEF_... in your step
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     *
     * @warning if you pass a modelName that correspond to a IN model the method returns the value of the method "firstItem(IN model)"
     *
     * @return NULL if no item will be found
     */
    CT_AbstractSingularItemDrawable* itemByOUTModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /**
     * @brief Returns the list of ItemDrawable corresponding to the possibilities of the IN model.
     *
     *        An IN model can have a number of possibilities > 1 if you set Choose_MultipleIfMultiple. A
     *        group can contains items that match with one or two or etc... possibility of the model so he must
     *        return a list of ItemDrawable.
     *
     * @return a empty list if no item will be found
     */
    virtual QList<CT_AbstractSingularItemDrawable*> items(const CT_InAbstractSingularItemModel *model) const = 0;

    /**
     * @brief Returns the list of ItemDrawable corresponding to the possibilities of the IN model to search by modelName
     *
     *        An IN model can have a number of possibilities > 1 if you set Choose_MultipleIfMultiple. A
     *        group can contains items that match with one or two or etc... possibility of the model so he must
     *        return a list of ItemDrawable.
     *
     * @param modelName : typically a DEF_... in your step
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     *
     * @warning if you pass a modelName that correspond to a OUT model the method returns the value of the method "item(OUT model)"
     *
     * @return a empty list if no item will be found
     */
    QList<CT_AbstractSingularItemDrawable*> itemsByINModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /**
     * @brief Returns the first ItemDrawable that match with a possibility of the IN model.
     *
     *        An IN model can have a number of possibilities > 1 if you set Choose_MultipleIfMultiple. A
     *        group can contains items that match with one or two or etc... possibility of the model so he must
     *        return a list of ItemDrawable. This method test the first possibility, if a ItemDrawable is found,
     *        it will be returned otherwise the method continue to search with the next possibility, etc...
     *
     * @return NULL if no item will be found
     */
    virtual CT_AbstractSingularItemDrawable* firstItem(const CT_InAbstractSingularItemModel *model) const = 0;

    /**
     * @brief Returns the ItemDrawable corresponding to the IN model to search by modelName
     * @param modelName : typically a DEF_... in your step
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     *
     * @warning if you pass a modelName that correspond to a OUT model the method returns the value of the method "item(OUT model)"
     *
     * @return NULL if no item will be found
     */
    CT_AbstractSingularItemDrawable* firstItemByINModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /****** GROUPS METHODS ********/

    /**
     * @brief Remove the group from this group. The group is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the group must be removed. This is a choice to help users who use multithreaded
     *        not worry about problems of access to groups removed from memory.
     *
     *        All methods that return the number of Groups will return the number of Groups that has not be
     *        removed (the flag is not set).
     *
     *        To test if a group is valid and will no be removed from memory call the method "willBeRemovedLater()".
     *
     *        All iterators don't returns those whose method willBeRemovedLater() return true
     */
    virtual bool removeGroup(CT_AbstractItemGroup *group) = 0;

    /**
     * @brief Returns a hashMap of CT_Container who contains groups. The key is the modelName.
     */
    virtual QHash<QString, CT_Container*> groups() const = 0;

    /**
     * @brief How many groups ?
     */
    virtual int nGroups() const = 0;

    /**
     * @brief Returns true if the group don't contains groups
     */
    virtual bool isEmptyOfGroups() const = 0;

    /**
     * @brief Returns true if the group contains a group with the out model passed in parameter
     */
    virtual bool containsGroup(const CT_OutAbstractGroupModel *model) const = 0;

    /**
     * @brief Returns the parentGroup of this group
     * @return NULL if the group is a root group (the parent group is the result)
     */
    CT_AbstractItemGroup* parentGroup() const;

    /**
     * @brief Return the container who contains this group
     */
    CT_Container* parentContainer() const;

    /**
     * @brief Returns true if the group will be removed at the end of the compute of your step
     */
    bool willBeRemovedLater() const;

    /**
     * @brief Returns true if at least one child must be removed later
     */
    bool atLeastOneChildMustBeRemovedLater() const;

    /**
     * @brief Set the parent group
     * @warning Not intended for direct use by plugin developper
     */
    void setParentGroup(const CT_AbstractItemGroup *pGroup);

    /**
     * @brief Set the container who contains this group
     * @warning Not intended for direct use by plugin developper
     */
    void setParentContainer(const CT_Container *container);

    /**
     * @brief Define a object to store in the group like a context for multithread processing. You must
     *        manage the life of this object in memory !
     */
    void setContext(const void *context);

    /**
     * @brief Returns the object stored in this group
     */
    void* getContext() const;

protected:

    friend class CT_ResultGroup;

    /**
     * @brief Called from result to inform that group that it must be removed later. Inform parent that this
     *        group must be removed later.
     */
    void setBeRemovedLater();

    /**
     * @brief Called from children to inform parent that at least one child must be removed later
     */
    void setAtLeastOneChildMustBeRemovedLater();

    /**
     * @brief Called from parent or result to delete all groups and items that must be removed later (recursively)
     * @warning Not intended for direct use by plugin developper
     */
    virtual void recursiveSetComplete() = 0;

    /**
     * @brief Called from a group if he want to call recursiveSetComplete() on childrens
     */
    void staticResursiveSetComplete(CT_AbstractItemGroup *group) { group->recursiveSetComplete(); }

    /**
     * @brief Overloaded to cast the model to check the validity.
     */
    virtual QString internalVerifyModel(const CT_OutAbstractModel *model) const;

private:
    RemoveLaterBits         _removedLater;  // quint8 : first bit is set if the group must be removed later, second bit is set if at least one child must be removed later

    CT_Container            *m_parentContainer;
    CT_AbstractItemGroup    *_parentGroup;
    void                    *_context;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CT_AbstractItemGroup::SearchMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(CT_AbstractItemGroup::RemoveLaterBits)

#endif // CT_ABSTRACTITEMGROUP_H
