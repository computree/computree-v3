#include "ct_tnodegroup.h"

#include "ct_ttreegroup.h"

CT_TNodeGroup::CT_TNodeGroup() : CT_StandardItemGroup()
{
    initConstructor();
}

CT_TNodeGroup::CT_TNodeGroup(const CT_OutAbstractGroupModel *model,
                             const CT_AbstractResult *result) : CT_StandardItemGroup(model, result)
{
    initConstructor();
}

CT_TNodeGroup::CT_TNodeGroup(const QString &modelName,
                             const CT_AbstractResult *result) : CT_StandardItemGroup(modelName, result)
{
    initConstructor();
}

CT_TNodeGroup::~CT_TNodeGroup()
{
}

QString CT_TNodeGroup::getType() const
{
    return staticGetType();
}

QString CT_TNodeGroup::staticGetType()
{
    return CT_StandardItemGroup::staticGetType() + "/CT_TNodeGroup";
}

QList<CT_AbstractItem *> CT_TNodeGroup::childrensForGui() const
{
    QList<CT_AbstractItem *> r;

    CT_TNodeGroup *o = m_rootComponent;

    if(o != NULL)
    {
        r.append(o);

        while(o->successor() != NULL)
        {
            o = o->successor();
            r.append(o);
        }
    }

    QListIterator<CT_TNodeGroup*> it(m_branches);

    while(it.hasNext())
        r.append(it.next());

    QHash<QString, CT_GroupItemDrawableContainer*> i = items();
    QHashIterator<QString, CT_GroupItemDrawableContainer*> itI(i);

    while(itI.hasNext())
        r.append(itI.next().value()->item());

    return r;
}

bool CT_TNodeGroup::setSuccessor(CT_TNodeGroup *successor)
{
    if(m_successor != NULL)
    {
        if(!recursiveRemoveNode(m_successor))
            return false;

        m_successor = NULL;
    }

    if(successor == NULL)
        return true;

    if(!addNode(successor))
        return false;

    m_successor = successor;
    successor->setAncestor(this);

    return true;
}

bool CT_TNodeGroup::addComponent(CT_TNodeGroup *component)
{
    if(m_rootComponent == NULL)
    {
        if(!addNode(component))
            return false;

        m_rootComponent = component;
        component->setComplex(this);

        return true;
    }

    CT_TNodeGroup *o = m_rootComponent;

    while(o->successor() != NULL)
        o = o->successor();

    return o->setSuccessor(component);
}

bool CT_TNodeGroup::addBranch(CT_TNodeGroup *son)
{
    if(!addNode(son))
        return false;

    m_branches.append(son);
    son->setBearer(this);

    return true;
}

bool CT_TNodeGroup::removeComponent(CT_TNodeGroup *component)
{
    if(component == m_rootComponent)
    {
        if(!recursiveRemoveNode(component))
            return false;

        m_rootComponent = NULL;
        return true;
    }

    if(component->ancestor() != NULL)
        return component->ancestor()->setSuccessor(NULL);

    return false;
}

bool CT_TNodeGroup::removeBranch(CT_TNodeGroup *son)
{
    if(m_branches.contains(son))
    {
        if(!recursiveRemoveNode(son))
            return false;

        return m_branches.removeOne(son);
    }

    return false;
}

CT_TNodeGroup* CT_TNodeGroup::ancestor() const
{
    return m_ancestor;
}

CT_TNodeGroup* CT_TNodeGroup::successor() const
{
    return m_successor;
}

CT_TNodeGroup* CT_TNodeGroup::complex() const
{
    return m_complex;
}

CT_TNodeGroup* CT_TNodeGroup::rootComponent() const
{
    return m_rootComponent;
}

CT_TNodeGroup* CT_TNodeGroup::bearer() const
{
    return m_bearer;
}

const QList<CT_TNodeGroup *>& CT_TNodeGroup::branches() const
{
    return m_branches;
}

size_t CT_TNodeGroup::nComponent() const
{
    size_t n = 0;

    CT_TNodeGroup *o = m_rootComponent;

    if(o != NULL)
    {
        ++n;

        while(o->successor() != NULL)
        {
            o = o->successor();
            ++n;
        }
    }

    return n;
}

CT_AbstractItemDrawable* CT_TNodeGroup::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    const CT_OutAbstractGroupModel *newModel = dynamic_cast< const CT_OutAbstractGroupModel* >(model);

    if(newModel == NULL)
        return NULL;

    CT_TNodeGroup *itemGroup = new CT_TNodeGroup(newModel, result);
    itemGroup->setId(id());
    itemGroup->setAlternativeDrawManager(getAlternativeDrawManager());

    // we can not copy successor, etc... because we don't know the model
	// it's not important because if the tree is copied it will inform all node
	// were is the successor, etc...

    // copy all ItemDrawable (reference) and other groups (copy)
    if(!copyStructure(itemGroup))
    {
        delete itemGroup;
        return NULL;
    }

    return itemGroup;
}

void CT_TNodeGroup::initConstructor()
{
    m_tree = NULL;

    m_successor = NULL;
    m_ancestor = NULL;

    m_complex = NULL;
    m_rootComponent = NULL;

    m_bearer = NULL;
}

CT_TTreeGroup* CT_TNodeGroup::tree() const
{
    return m_tree;
}

void CT_TNodeGroup::setAncestor(CT_TNodeGroup *o)
{
    m_ancestor = o;
}

void CT_TNodeGroup::setComplex(CT_TNodeGroup *o)
{
    m_complex = o;
}

void CT_TNodeGroup::setBearer(CT_TNodeGroup *o)
{
    m_bearer = o;
}

bool CT_TNodeGroup::addNode(CT_TNodeGroup *n)
{
    if(n->tree() == NULL)
        n->setTopologyTree(tree());

    return tree()->addNode(n);
}

bool CT_TNodeGroup::recursiveRemoveNode(CT_TNodeGroup *n)
{
    if(tree() == NULL)
        return false;

    CT_TNodeGroup *s = n->rootComponent();

    if(s != NULL)
    {
        if(!recursiveRemoveNode(s))
            return false;
    }

    s = n->successor();

    if(s != NULL)
    {
        if(!recursiveRemoveNode(s))
            return false;
    }

    QListIterator<CT_TNodeGroup*> it(n->branches());

    while(it.hasNext())
    {
        if(!recursiveRemoveNode(it.next()))
            return false;
    }

    return tree()->removeNode(n);
}

void CT_TNodeGroup::setTopologyTree(CT_TTreeGroup *tree)
{
    m_tree = tree;
}

