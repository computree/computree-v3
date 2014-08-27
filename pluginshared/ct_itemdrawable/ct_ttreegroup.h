#ifndef CT_TTREEGROUP_H
#define CT_TTREEGROUP_H

#include "ct_itemdrawable/ct_tnodegroup.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

#include "ct_itemdrawable/ct_container.h"

/**
 * @brief A class that represent the root of a topology tree. You can only add CT_TNodeGroup in this
 *        group.
 *
 *        The model must be implemented like this :
 *
 *        CT_TTreeGroup (must contains all different types that you want to add in the topology)
 *          |
 *          |-- CT_TNodeGroup (first type, per example Axis)
 *          |       |
 *          |       |-- CT_MeshModel (a mesh model or a CT_AbstractSingularItemDrawable that represent the shape, not obligatory)
 *          |       |-- CT_MetricT<?> (one or multiple metric that represent a attribut if you want to add a attribute in the topology, not obligatory)
 *          |       |-- etc...
 *          |
 *          |-- CT_TNodeGroup (second type, per example Internode)
 *          |
 *          |-- etc ... (other types)
 *
 *        When you add a group with the method setRootNode or addSuccessor or addComponent, etc... this class verify if the
 *        group exist in the model and add them if the test pass.
 */
class PLUGINSHAREDSHARED_EXPORT CT_TTreeGroup : public CT_AbstractStandardItemGroup
{
    Q_OBJECT

public:
    CT_TTreeGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_TTreeGroup(const CT_OutAbstractGroupModel *model,
                  const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_TTreeGroup(const QString &modelName,
                  const CT_AbstractResult *result);
    ~CT_TTreeGroup();

    QString getType() const;
    static QString staticGetType();

    /*!
     *  \brief Always returns false. You can not add a item in this type of group. Item will be destroyed from memory.
     */
    bool addItemDrawable(CT_AbstractSingularItemDrawable *item) { delete item; return false; }

    bool setRootNode(CT_TNodeGroup *root);

    bool addSuccessor(CT_TNodeGroup *current, CT_TNodeGroup *successor);

    bool addComponent(CT_TNodeGroup *complex, CT_TNodeGroup *component);

    bool addBranch(CT_TNodeGroup *bearer, CT_TNodeGroup *son);

    CT_TNodeGroup* rootNode() const;

    // Inherited from ItemDrawable //
    virtual bool hasChildren() const;
    virtual bool beginIterateChild();
    virtual CT_AbstractItemDrawable* nextChild();

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    CT_TNodeGroup   *m_rootNode;
    bool            m_beginIterateChild;

    CT_TNodeGroup* recursiveCopyNodes(const CT_OutAbstractGroupModel *newModel,
                                      const CT_AbstractResult *result,
                                      CT_TTreeGroup *group,
                                      CT_TNodeGroup *node,
                                      const QHash<QString, CT_OutAbstractGroupModel*> &modelsMap);

protected:
    friend class CT_TNodeGroup;

    /**
     * @brief Add the node to the structure
     */
    bool addNode(CT_TNodeGroup *n);

    /**
     * @brief Remove the node from the structure
     * @warning if the node has children (successor, etc...) you must call this method for all childrens !!!
     */
    bool removeNode(CT_TNodeGroup *n);
};

#endif // CT_TTREEGROUP_H
