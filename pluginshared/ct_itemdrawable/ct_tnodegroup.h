#ifndef CT_TNODEGROUP_H
#define CT_TNODEGROUP_H

#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

class CT_TTreeGroup;

/**
 * @brief A class that you can use with a CT_TTreeGroup to create a topology.
 */
class PLUGINSHAREDSHARED_EXPORT CT_TNodeGroup : public CT_StandardItemGroup
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_TNodeGroup, CT_StandardItemGroup, Node group)

public:
    CT_TNodeGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_TNodeGroup(const CT_OutAbstractGroupModel *model,
                  const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_TNodeGroup(const QString &modelName,
                  const CT_AbstractResult *result);
    ~CT_TNodeGroup();

    /**
     * @brief Returns childrens of this group for the GUI
     */
    QList<CT_AbstractItem*> childrensForGui() const;

    /**
     * @brief Set the successor of the node
     * @warning if a successor already exist it will be removed and can not be used after (it will be deleted from memory at any moment)
     */
    bool setSuccessor(CT_TNodeGroup *successor);

    /**
     * @brief Set the root component or if already exist set the successor to the end of successor of the list
     */
    bool addComponent(CT_TNodeGroup *component);

    /**
     * @brief Add a branch to the list
     */
    bool addBranch(CT_TNodeGroup *son);

    /**
     * @brief Remove the component passed in parameter
     * @param recursively : if true all successor of this component will be removed too
     * @warning removed component can not be used after (it will be deleted from memory at any moment)
     */
    bool removeComponent(CT_TNodeGroup *component, bool recursively = true);

    /**
     * @brief Remove the branch passed in parameter
     * @warning removed branch can not be used after (it will be deleted from memory at any moment)
     */
    bool removeBranch(CT_TNodeGroup *son);

    /**
     * @brief Returns the ancestor of this node
     */
    CT_TNodeGroup* ancestor() const;

    /**
     * @brief Returns the successor of this node
     */
    CT_TNodeGroup* successor() const;

    /**
     * @brief Returns the complex of this node
     */
    CT_TNodeGroup* complex() const;

    /**
     * @brief Returns the root component of this node
     */
    CT_TNodeGroup* rootComponent() const;

    /**
     * @brief Returns the bearer of this node
     */
    CT_TNodeGroup* bearer() const;

    /**
     * @brief Returns branches of this node
     */
    const QList<CT_TNodeGroup*>& branches() const;

    /**
     * @brief Returns the number of component of this node
     */
    size_t nComponent() const;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    CT_TTreeGroup           *m_tree;

    CT_TNodeGroup           *m_ancestor;
    CT_TNodeGroup           *m_successor;

    CT_TNodeGroup           *m_complex;
    CT_TNodeGroup           *m_rootComponent;

    CT_TNodeGroup           *m_bearer;
    QList<CT_TNodeGroup*>   m_branches;

    QList<CT_TNodeGroup*>   m_iterateList;

protected:
    void initConstructor();

    CT_TTreeGroup* tree() const;

    void internalSetSuccessor(CT_TNodeGroup *successor);

    void setAncestor(CT_TNodeGroup *o);
    void setComplex(CT_TNodeGroup *o);
    void setBearer(CT_TNodeGroup *o);

    bool addNode(CT_TNodeGroup *n);
    bool recursiveRemoveNode(CT_TNodeGroup *n);

    friend class CT_TTreeGroup;
    void setTopologyTree(CT_TTreeGroup *tree);
};

#endif // CT_TNODEGROUP_H
