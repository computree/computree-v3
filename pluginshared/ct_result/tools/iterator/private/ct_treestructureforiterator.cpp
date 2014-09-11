#include "ct_treestructureforiterator.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

CT_TreeStructureForIterator::CT_TreeStructureForIterator()
{
    m_iterator = NULL;
    m_currentItem = NULL;
    m_currentIA = NULL;

    _model = NULL;
    _parent = NULL;
    _currentChild = NULL;

    _childListIndex = 0;
    _nChild = 0;
    m_toSearch = false;

    m_modelIsAGroup = true;
    m_modelIsAItemAttribute = false;

    resetIncrementChildIndexSuccess();
}

CT_TreeStructureForIterator::~CT_TreeStructureForIterator()
{
    clear();
}

void CT_TreeStructureForIterator::addChild(CT_TreeStructureForIterator *child)
{
    if(_currentChild == NULL)
        _currentChild = child;

    child->setParent(this);
    _children.append(child);

    ++_nChild;
}

void CT_TreeStructureForIterator::setModel(const CT_OutAbstractModel *model)
{
    _model = (CT_OutAbstractModel*)model;
    m_modelIsAGroup = (dynamic_cast<const CT_OutAbstractGroupModel*>(model) != NULL);
    m_modelIsAItemAttribute = (dynamic_cast<const CT_OutAbstractItemAttributeModel*>(model) != NULL);
}

CT_OutAbstractModel* CT_TreeStructureForIterator::outModel() const
{
    return _model;
}

void CT_TreeStructureForIterator::setToSearch(bool enable)
{
    m_toSearch = enable;
}

bool CT_TreeStructureForIterator::isSearched() const
{
    return m_toSearch;
}

bool CT_TreeStructureForIterator::isAGroup() const
{
    return m_modelIsAGroup;
}

bool CT_TreeStructureForIterator::isAItemAttribute() const
{
    return m_modelIsAItemAttribute;
}

CT_TreeStructureForIterator* CT_TreeStructureForIterator::getChildWithModel(CT_AbstractModel *model) const
{
    QListIterator<CT_TreeStructureForIterator*> it(_children);

    while(it.hasNext())
    {
        CT_TreeStructureForIterator *child = it.next();

        if(child->outModel()->uniqueName() == model->uniqueName())
            return child;
    }

    return NULL;
}

const QList<CT_TreeStructureForIterator*>& CT_TreeStructureForIterator::children() const
{
    return _children;
}

bool CT_TreeStructureForIterator::incrementChildIndexAndSetCurrentChild()
{
    ++_childListIndex;

    if(_childListIndex >= _nChild)
    {
        resetChildIndexAndSetCurrentChild();
        _incrementChildIndexSuccess = false;
        return false;
    }

    _currentChild = _children.at(_childListIndex);

    return true;
}

bool CT_TreeStructureForIterator::isLastIncrementChildIndexSuccess() const
{
    return _incrementChildIndexSuccess;
}

void CT_TreeStructureForIterator::resetIncrementChildIndexSuccess()
{
    _incrementChildIndexSuccess = true;
}

void CT_TreeStructureForIterator::resetChildIndexAndSetCurrentChild()
{
    _childListIndex = 0;
    resetIncrementChildIndexSuccess();

    if(_nChild > 0)
        _currentChild = _children.first();
    else
        _currentChild = NULL;
}

int CT_TreeStructureForIterator::nChildren() const
{
    return _nChild;
}

CT_TreeStructureForIterator* CT_TreeStructureForIterator::currentChild() const
{
    return _currentChild;
}

void CT_TreeStructureForIterator::setIterator(void *c)
{
    m_iterator = c;
}

void* CT_TreeStructureForIterator::iterator() const
{
    return m_iterator;
}

void CT_TreeStructureForIterator::setCurrentItem(const CT_AbstractItemDrawable *item)
{
    m_currentItem = (CT_AbstractItemDrawable*)item;
}

void CT_TreeStructureForIterator::setCurrentItemAttribute(const CT_AbstractItemAttribute *ia)
{
    m_currentIA = (CT_AbstractItemAttribute*)ia;
}

CT_AbstractItemDrawable* CT_TreeStructureForIterator::currentItem() const
{
    return m_currentItem;
}

CT_AbstractItemAttribute* CT_TreeStructureForIterator::currentItemAttribute() const
{
    return m_currentIA;
}

void CT_TreeStructureForIterator::clear()
{
    qDeleteAll(_children.begin(), _children.end());
    _children.clear();

    _nChild = 0;
    resetChildIndexAndSetCurrentChild();
}

CT_TreeStructureForIterator* CT_TreeStructureForIterator::parent() const
{
    return _parent;
}

void CT_TreeStructureForIterator::setParent(CT_TreeStructureForIterator *parent)
{
    _parent = parent;
}

// CT_TreeStructureConstructor //

CT_TreeStructureConstructor::CT_TreeStructureConstructor()
{
    _structure = NULL;
}

CT_TreeStructureConstructor::~CT_TreeStructureConstructor()
{
    clearTreeStructure();
}

bool CT_TreeStructureConstructor::addModelsForItemAttributesToStructure(CT_OutAbstractItemAttributeModel *outIAModel)
{
    if(outIAModel->parentModel() == NULL)
        return false;

    QList<CT_OutAbstractModel*> models;

    // item attribute
    models.append(outIAModel);

    // item
    models.append((CT_OutAbstractModel*)outIAModel->parentModel());

    // group
    DEF_CT_AbstractGroupModelOut *parentGroup = dynamic_cast< DEF_CT_AbstractGroupModelOut* >(outIAModel->parentModel()->parentModel());

    // on crée la liste des groupes modèles en partant du groupe parent de l'item pour aller jusqu'au groupe racine (en remontant dans la structure)
    while(parentGroup != NULL)
    {
        models.insert(0, parentGroup);
        parentGroup = parentGroup->parentGroup();
    }

    if(models.size() == 1)
        return false;

    return mergeWithCurrentStructure(models);
}

bool CT_TreeStructureConstructor::addModelsForItemsToStructure(CT_OutAbstractSingularItemModel *outItemModel)
{
    QList<CT_OutAbstractModel*> models;

    models.append(outItemModel);

    DEF_CT_AbstractGroupModelOut *parentGroup = dynamic_cast< DEF_CT_AbstractGroupModelOut* >(outItemModel->parentModel());

    // on crée la liste des groupes modèles en partant du groupe parent de l'item pour aller jusqu'au groupe racine (en remontant dans la structure)
    while(parentGroup != NULL)
    {
        models.insert(0, parentGroup);
        parentGroup = parentGroup->parentGroup();
    }

    if(models.size() == 1)
        return false;

    return mergeWithCurrentStructure(models);
}

bool CT_TreeStructureConstructor::addModelsForGroupsToStructure(DEF_CT_AbstractGroupModelOut *outGroupModel)
{
    QList<CT_OutAbstractModel*> models;

    DEF_CT_AbstractGroupModelOut *parentGroup = outGroupModel;

    // on crée la liste des groupes modèles en partant du groupe parent de l'item pour aller jusqu'au groupe racine (en remontant dans la structure)
    while(parentGroup != NULL)
    {
        models.insert(0, parentGroup);
        parentGroup = parentGroup->parentGroup();
    }

    if(models.isEmpty())
        return false;

    return mergeWithCurrentStructure(models);
}

bool CT_TreeStructureConstructor::addModelsForItemAttributesToStructure(CT_InStdModelPossibility *itemModelPossibility)
{
    CT_OutAbstractItemAttributeModel *iaModel = dynamic_cast<CT_OutAbstractItemAttributeModel*>(itemModelPossibility->outModel());

    if(iaModel == NULL)
        return false;

    return addModelsForItemAttributesToStructure(iaModel);
}

bool CT_TreeStructureConstructor::addModelsForItemsToStructure(CT_InStdModelPossibility *itemModelPossibility)
{
    CT_OutAbstractSingularItemModel *itemModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(itemModelPossibility->outModel());

    if(itemModel == NULL)
        return false;

    return addModelsForItemsToStructure(itemModel);
}

bool CT_TreeStructureConstructor::addModelsForGroupsToStructure(CT_InStdModelPossibility *groupModelPossibility)
{
    DEF_CT_AbstractGroupModelOut *parentGroup = dynamic_cast< DEF_CT_AbstractGroupModelOut* >(groupModelPossibility->outModel());

    if(parentGroup == NULL)
        return false;

    return addModelsForGroupsToStructure(parentGroup);
}

void CT_TreeStructureConstructor::clearTreeStructure()
{
    delete _structure;
    _structure = NULL;
}

CT_TreeStructureForIterator* CT_TreeStructureConstructor::takeStructure()
{
    CT_TreeStructureForIterator *st = _structure;
    _structure = NULL;

    return st;
}

bool CT_TreeStructureConstructor::mergeWithCurrentStructure(const QList<CT_OutAbstractModel*> &models)
{
    if(_structure == NULL)
    {
        _structure = new CT_TreeStructureForIterator();
        _structure->setModel(models.first());
    }

    CT_TreeStructureForIterator *currentChild = _structure;

    QListIterator<CT_OutAbstractModel*> it(models);
    it.next();

    if(!it.hasNext())
    {
        currentChild->setToSearch(true);
    }
    else
    {
        while(it.hasNext())
        {
            CT_OutAbstractModel *model = it.next();

            CT_TreeStructureForIterator *newChild = currentChild->getChildWithModel(model);

            if(newChild == NULL)
            {
                newChild = new CT_TreeStructureForIterator();
                newChild->setModel(model);

                currentChild->addChild(newChild);
            }

            if(!it.hasNext())
                newChild->setToSearch(true);

            currentChild = newChild;
        }
    }

    return true;
}
