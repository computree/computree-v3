#include "ct_ttreegroup.h"

CT_TTreeGroup::CT_TTreeGroup() : CT_AbstractStandardItemGroup()
{
    m_rootNode = NULL;
}

CT_TTreeGroup::CT_TTreeGroup(const CT_OutAbstractGroupModel *model,
                             const CT_AbstractResult *result) : CT_AbstractStandardItemGroup(model, result)
{
    m_rootNode = NULL;
}

CT_TTreeGroup::CT_TTreeGroup(const QString &modelName,
                             const CT_AbstractResult *result) : CT_AbstractStandardItemGroup(modelName, result)
{
    m_rootNode = NULL;
}

CT_TTreeGroup::~CT_TTreeGroup()
{
}

QList<CT_AbstractItem *> CT_TTreeGroup::childrensForGui() const
{
    QList<CT_AbstractItem *> r;

    if(m_rootNode != NULL)
        r << m_rootNode;

    return r;
}

bool CT_TTreeGroup::setRootNode(CT_TNodeGroup *root)
{
    if(m_rootNode != NULL)
    {
        if(!removeNode(m_rootNode))
            return false;

        m_rootNode = NULL;
    }

    if(!addNode(root))
        return false;

    m_rootNode = root;
    m_rootNode->setTopologyTree(this);

    return true;
}

bool CT_TTreeGroup::addSuccessor(CT_TNodeGroup *current, CT_TNodeGroup *successor)
{
    return current->setSuccessor(successor);
}

bool CT_TTreeGroup::addComponent(CT_TNodeGroup *complex, CT_TNodeGroup *component)
{
    return complex->addComponent(component);
}

bool CT_TTreeGroup::addBranch(CT_TNodeGroup *bearer, CT_TNodeGroup *son)
{
    return bearer->addBranch(son);
}

CT_TNodeGroup* CT_TTreeGroup::rootNode() const
{
    return m_rootNode;
}

CT_AbstractItemDrawable* CT_TTreeGroup::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    const CT_OutAbstractGroupModel *newModel = dynamic_cast< const CT_OutAbstractGroupModel* >(model);

    CT_TTreeGroup *itemGroup = new CT_TTreeGroup(newModel, result);
    itemGroup->setId(id());
    itemGroup->setAlternativeDrawManager(getAlternativeDrawManager());

    if(newModel == NULL)
        return itemGroup;

    // create the hash map of new models [old model, new model]
    if(m_rootNode != NULL)
    {
        QHash<QString, CT_OutAbstractGroupModel*> modelsMap;

        QListIterator<DEF_CT_AbstractGroupModelOut*> it(newModel->groups());

        while(it.hasNext())
        {
            DEF_CT_AbstractGroupModelOut *mm = it.next();

            if(CT_TNodeGroup::staticGetType() == mm->itemDrawable()->getType())
                modelsMap.insert(mm->uniqueName(), (CT_OutAbstractGroupModel*)mm);
        }

        // we must copy recursively all nodes and set it the new model
        CT_TNodeGroup *root = recursiveCopyNodes(newModel, result, itemGroup, m_rootNode, modelsMap);

        if((root == NULL) || !itemGroup->setRootNode(root))
        {
            delete itemGroup;
            itemGroup = NULL;
        }
    }

    return itemGroup;
}

CT_TNodeGroup* CT_TTreeGroup::recursiveCopyNodes(const CT_OutAbstractGroupModel *newModel,
                                                 const CT_AbstractResult *result,
                                                 CT_TTreeGroup *group,
                                                 CT_TNodeGroup *node,
                                                 const QHash<QString, CT_OutAbstractGroupModel*> &modelsMap)
{
    CT_OutAbstractGroupModel *newNodeModel;

    if((newNodeModel = modelsMap.value(node->model()->uniqueName(), NULL)) == NULL)
        return NULL;

    CT_TNodeGroup *newNode = (CT_TNodeGroup*)node->copy(newNodeModel, result, CT_ResultCopyModeList());
    newNode->setTopologyTree(group);

    CT_TNodeGroup *successor = node->successor();

    if(successor != NULL)
    {
        successor = recursiveCopyNodes(newModel, result, group, successor, modelsMap);

        if((successor == NULL) || (!newNode->setSuccessor(successor)))
        {
            delete newNode;
            delete successor;
            return NULL;
        }
    }

    CT_TNodeGroup *rootComponent = node->rootComponent();

    if(rootComponent != NULL)
    {
        rootComponent = recursiveCopyNodes(newModel, result, group, rootComponent, modelsMap);

        if((rootComponent == NULL) || (!newNode->addComponent(rootComponent)))
        {
            delete newNode;
            delete rootComponent;
            return NULL;
        }
    }

    QListIterator<CT_TNodeGroup*> it(node->branches());

    while(it.hasNext())
    {
        CT_TNodeGroup *branch = it.next();

        branch = recursiveCopyNodes(newModel, result, group, branch, modelsMap);

        if((branch == NULL) || (!newNode->addBranch(branch)))
        {
            delete newNode;
            delete branch;
            return NULL;
        }
    }

    return newNode;
}

bool CT_TTreeGroup::addNode(CT_TNodeGroup *n)
{
    return protectedAddGroup(n);
}

bool CT_TTreeGroup::removeNode(CT_TNodeGroup *n)
{
    return removeGroup(n);
}
