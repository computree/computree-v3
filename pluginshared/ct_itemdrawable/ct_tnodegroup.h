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

    QString getType() const;
    static QString staticGetType();

    /**
     * @brief Set the successor of the node
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

    CT_TNodeGroup* ancestor() const;
    CT_TNodeGroup* successor() const;

    CT_TNodeGroup* complex() const;
    CT_TNodeGroup* rootComponent() const;

    CT_TNodeGroup* bearer() const;
    const QList<CT_TNodeGroup*>& branches() const;

    size_t nComponent() const;

    // Inherited from ItemDrawable //
    virtual bool hasChildren() const;
    virtual bool beginIterateChild();
    virtual CT_AbstractItemDrawable* nextChild();

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

    void setAncestor(CT_TNodeGroup *o);
    void setComplex(CT_TNodeGroup *o);
    void setBearer(CT_TNodeGroup *o);

    bool addNode(CT_TNodeGroup *n);
    bool recursiveRemoveNode(CT_TNodeGroup *n);

    friend class CT_TTreeGroup;
    void setTopologyTree(CT_TTreeGroup *tree);
};

#endif // CT_TNODEGROUP_H
