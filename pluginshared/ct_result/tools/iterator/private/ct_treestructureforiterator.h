#ifndef CT_TREESTRUCTUREFORITERATOR_H
#define CT_TREESTRUCTUREFORITERATOR_H

#include "def_ct_abstractgroupmodelout.h"
#include <QList>

class CT_TreeStructureConstructor;
class CT_InStdModelPossibility;
class CT_AbstractItemAttribute;

/**
 * @brief Show the class below (CT_TreeStructureConstructor) for more information
 */
class PLUGINSHAREDSHARED_EXPORT CT_TreeStructureForIterator
{
public:
    CT_TreeStructureForIterator();
    ~CT_TreeStructureForIterator();

    /**
     * @brief To know if the model of this node is searched
     */
    bool isSearched() const;

    /**
     * @brief Returns true if the model is a CT_OutAbstractGroupModel
     */
    bool isAGroup() const;

    /**
     * @brief Returns true if the model is a CT_OutAbstractItemAttributeModel
     */
    bool isAItemAttribute() const;

    /**
     * @brief Returns the parent of this node
     */
    CT_TreeStructureForIterator* parent() const;

    /**
     * @brief Returns the model of this node
     */
    CT_OutAbstractModel* outModel() const;

    /**
     * @brief Returns children of this node
     */
    const QList<CT_TreeStructureForIterator*>& children() const;

    /**
     * @brief Returns number of child
     */
    int nChildren() const;

    /**
     * @brief This class can help you to know and backup where you are in the structure.
     *
     *        Set the current child to the first child
     */
    void resetChildIndexAndSetCurrentChild();

    /**
     * @brief This class can help you to know and backup where you are in the structure.
     *
     *        Go to next child
     */
    bool incrementChildIndexAndSetCurrentChild();

    /**
     * @brief This class can help you to know and backup where you are in the structure.
     *
     *        Returns true if go to next child succeeded
     */
    bool isLastIncrementChildIndexSuccess() const;

    /**
     * @brief This class can help you to know and backup where you are in the structure.
     *
     *        Reset the flag to know if the last increment of child succeeded (set to true)
     */
    void resetIncrementChildIndexSuccess();

    /**
     * @brief This class can help you to know and backup where you are in the structure.
     *
     *        Returns current child
     */
    CT_TreeStructureForIterator* currentChild() const;

    /**
     * @brief You can set a iterator if you want
     */
    void setIterator(void* c);

    /**
     * @brief Returns the iterator
     */
    void* iterator() const;

    /**
     * @brief You can set the current item of the iterator
     */
    void setCurrentItem(const CT_AbstractItemDrawable *item);

    /**
     * @brief You can set the current item attribute of the iterator
     */
    void setCurrentItemAttribute(const CT_AbstractItemAttribute *ia);

    /**
     * @brief Return the current item
     */
    CT_AbstractItemDrawable* currentItem() const;

    /**
     * @brief Return the current item attribute
     */
    CT_AbstractItemAttribute* currentItemAttribute() const;

protected:
    friend class CT_TreeStructureConstructor;

    void addChild(CT_TreeStructureForIterator *child);

    void setModel(const CT_OutAbstractModel *model);

    void setToSearch(bool enable);

    CT_TreeStructureForIterator* getChildWithModel(CT_AbstractModel *model) const;

private:

    void                                *m_iterator;
    CT_AbstractItemDrawable             *m_currentItem;
    CT_AbstractItemAttribute            *m_currentIA;

    CT_TreeStructureForIterator         *_parent;
    QList<CT_TreeStructureForIterator*> _children;
    CT_TreeStructureForIterator         *_currentChild;

    CT_OutAbstractModel                 *_model;
    bool                                m_modelIsAGroup;
    bool                                m_modelIsAItemAttribute;

    int                                 _nChild;
    int                                 _childListIndex;
    bool                                _incrementChildIndexSuccess;
    bool                                m_toSearch;

    void setParent(CT_TreeStructureForIterator *parent);
    void clear();
};

/**
 * @brief A tool class that construct a tree of CT_TreeStructureForIterator. You can use
 *        this class to know what possibilities of a IN model is searched.
 *
 *        example (constructed with a IN model for SingularItemDrawable):
 *
 *          [G0] (root ---> isSearched() return false)
 *           |---[G1] (child to enter ---> isSearched() return false)
 *           |    |----[I] (child to search ---> isSearched() return true)
 *           |
 *           |---[G2] (child to enter ---> isSearched() return false)
 *                |----[I] (child to search ---> isSearched() return true)
 *
 *
 *        example (constructed with a IN model for Group):
 *
 *          [G0] (root ---> isSearched() return false)
 *           |---[G1] (child to iterate ---> isSearched() return true)
 *           |
 *           |---[G2] (child to enter ---> isSearched() return false)
 *                |----[G3] (child to search ---> isSearched() return true)
 *
 */
class CT_TreeStructureConstructor
{
public:

    CT_TreeStructureConstructor();
    ~CT_TreeStructureConstructor();

    /**
     * @brief First method to call. Pass the OUT model of your choice. The method
     *        merge the structure with the model's structure.
     */
    bool addModelsForItemAttributesToStructure(CT_OutAbstractItemAttributeModel *outIAModel);

    /**
     * @brief First method to call. Pass the OUT model of your choice. The method
     *        merge the structure with the model's structure.
     */
    bool addModelsForItemsToStructure(CT_OutAbstractSingularItemModel *outItemModel);

    /**
     * @brief First method to call. Pass the possibility of the OUT model of your choice. The method
     *        merge the structure with the model's structure.
     */
    bool addModelsForGroupsToStructure(DEF_CT_AbstractGroupModelOut *outGroupModel);

    /**
     * @brief First method to call. Pass the possibility of the IN model of your choice. The method
     *        merge the structure with the model's structure.
     *
     * @warning the model of the possibility must be a CT_OutAbstractItemAttributeModel (singular item drawable)
     */
    bool addModelsForItemAttributesToStructure(CT_InStdModelPossibility *itemModelPossibility);

    /**
     * @brief First method to call. Pass the possibility of the IN model of your choice. The method
     *        merge the structure with the model's structure.
     *
     * @warning the model of the possibility must be a CT_OutAbstractSingularItemModel (singular item drawable)
     */
    bool addModelsForItemsToStructure(CT_InStdModelPossibility *itemModelPossibility);

    /**
     * @brief First method to call. Pass the possibility of the IN model of your choice. The method
     *        merge the structure with the model's structure.
     *
     * @warning the model of the possibility must be a DEF_CT_AbstractGroupModelOut (group)
     */
    bool addModelsForGroupsToStructure(CT_InStdModelPossibility *groupModelPossibility);

    /**
     * @brief Clear the tree structure created.
     */
    void clearTreeStructure();

    /**
     * @brief Take the tree structure created by the class.
     */
    CT_TreeStructureForIterator* takeStructure();

private:

    CT_TreeStructureForIterator *_structure;

    bool mergeWithCurrentStructure(const QList<CT_OutAbstractModel *> &models);
};

#endif // CT_TREESTRUCTUREFORITERATOR_H
