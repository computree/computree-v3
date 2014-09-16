#ifndef CT_ABSTRACTGROUPMODELT_HPP
#define CT_ABSTRACTGROUPMODELT_HPP

#include "ct_abstractgroupmodelt.h"
#include "ct_model/abstract/ct_abstractmodel.h"

#include <QVector>

template< class ItemT, class InheritedT >
CT_AbstractGroupModelT<ItemT,InheritedT>::CT_AbstractGroupModelT(const QString &uniqueName,
                                                                 const QString &description,
                                                                 const QString &displayableName) : InheritedT(uniqueName,
                                                                                                              description,
                                                                                                              displayableName)
{
}

template< class ItemT, class InheritedT >
CT_AbstractGroupModelT<ItemT,InheritedT>::~CT_AbstractGroupModelT()
{
    clear();
}

template< class ItemT, class InheritedT >
CT_AbstractGroupModelT<ItemT,InheritedT>* CT_AbstractGroupModelT<ItemT,InheritedT>::parentGroup() const
{
    return dynamic_cast< CT_AbstractGroupModelT<ItemT,InheritedT>* >(this->parentModel());
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::addGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group)
{
    if(this->internalExistModelOrChildrensInTree(rootGroup(), group)
            || group->uniqueName().isEmpty())
    {
        delete group;
        return false;
    }

    addGroupWithoutVerification(group);

    return true;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::addGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group, CT_AutoRenameModels &autoName)
{
    if(!group->isEmpty())
    {
        delete group;
        return false;
    }

    CT_AbstractGroupModelT<ItemT,InheritedT> *root = rootGroup();

    // si on a déjà attribué un nouveau nom à un groupe précédent
    if(autoName.hasBeenModified())
    {
        // on attribue ce nom au nouveau groupe
        group->setUniqueName(autoName.completeName());

        // on vérifie qu'il n'existe pas déjà dans notre arborescence
        if(!this->internalExistModelOrChildrensInTree(root, group))
        {
            // si c'est bon on l'ajoute
            addGroupWithoutVerification(group);
            autoName.addModel(group);
            return true;
        }
    }
    else // sinon
    {
        // on met un nom par défaut
        autoName.setName("g");
        autoName.setExtra(0);

        group->setUniqueName(autoName.completeName());
    }

    // tant que le groupe existe avec ce nom dans l'arborescence
    // ou tant qu'on ne peut pas renommer les anciens groupes avec
    // ce nouveau nom
    while(this->internalExistModelOrChildrensInTree(root,group)
          || !autoName.renameAllModels())
    {
        autoName.setExtra(autoName.extra()+1);
        group->setUniqueName(autoName.completeName());
    }

    // on ajoute ce modèle à l'AutoName
    autoName.addModel(group);

    addGroupWithoutVerification(group);

    return true;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::removeGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group)
{
    if(_groups.removeOne(group))
    {
        groupRemoved(group);

        delete group;
        return true;
    }

    return false;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::addItem(ItemT *item)
{
    if(this->internalExistModelOrChildrensInTree(rootGroup(), item)
            || item->uniqueName().isEmpty())
    {
        delete item;

        return false;
    }

    addItemWithoutVerification(item);

    return true;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::addItem(ItemT *item, CT_AutoRenameModels &autoName)
{
    CT_AbstractGroupModelT<ItemT,InheritedT> *root = rootGroup();

    // si on a déjà attribué un nouveau nom à un item précédent
    if(!autoName.models().isEmpty())
    {
        // on regarde si un des anciens modèles à le même groupe racine que notre groupe racine (compare les adresses)
        ItemT *ancModel = dynamic_cast<ItemT*>(autoName.models().first());

        // si l'ancien modèle n'est pas un item
        // ou si son groupe racine est égual à notre groupe racine
        if((ancModel == NULL)
                || (ancModel->rootModel() == rootGroup()))
        {
            // alors on n'ajoute pas le nouvel item car on ne peut pas avoir
            // deux items ayant le même nom dans la même arborescence
            delete item;

            // Il faut un autre CT_AutoRenameModels pour pouvoir ajouter cet item
            return false;
        }

        // sinon on attribue le nom déjà défini au nouveau groupe
        item->setUniqueName(autoName.completeName());

        // on vérifie qu'il n'existe pas déjà dans notre arborescence
        if(!this->internalExistModelOrChildrensInTree(root, item))
        {
            // si c'est bon on l'ajoute
            addItemWithoutVerification(item);
            autoName.addModel(item);
            return true;
        }
    }
    else // sinon
    {
        // on met un nom par défaut
        autoName.setName("i");
        autoName.setExtra(0);

        item->setUniqueName(autoName.completeName());
    }

    // tant que l'item existe avec ce nom dans l'arborescence
    // ou tant qu'on ne peut pas renommer les anciens items avec
    // ce nouveau nom
    while(this->internalExistModelOrChildrensInTree(root, item)
          || !autoName.renameAllModels())
    {
        // on renomme le nouvel item
        autoName.setExtra(autoName.extra()+1);
        item->setUniqueName(autoName.completeName());
    }

    // on ajoute cet item modèle à l'AutoName
    autoName.addModel(item);

    // on ajoute cet item modèle à notre liste
    addItemWithoutVerification(item);

    return true;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::removeItem(ItemT *item)
{
    if(_items.removeOne(item))
    {
        itemRemoved(item);

        delete item;
        return true;
    }

    return false;
}

template< class ItemT, class InheritedT >
const QList<CT_AbstractGroupModelT<ItemT,InheritedT>*>& CT_AbstractGroupModelT<ItemT,InheritedT>::groups() const
{
    return _groups;
}

template< class ItemT, class InheritedT >
const QList<ItemT*>& CT_AbstractGroupModelT<ItemT,InheritedT>::items() const
{
    return _items;
}

template< class ItemT, class InheritedT >
CT_AbstractGroupModelT<ItemT,InheritedT>* CT_AbstractGroupModelT<ItemT,InheritedT>::findGroup(const QString &modelName) const
{
    QListIterator< CT_AbstractGroupModelT<ItemT,InheritedT>* > it(groups());

    while(it.hasNext())
    {
        CT_AbstractGroupModelT<ItemT,InheritedT> *group = it.next();

        if(group->uniqueName() == modelName)
            return group;
    }

    return NULL;
}

template< class ItemT, class InheritedT >
ItemT* CT_AbstractGroupModelT<ItemT,InheritedT>::findItem(const QString &modelName) const
{
    QListIterator<ItemT*> it(items());

    while(it.hasNext())
    {
        ItemT *item = it.next();

        if(item->uniqueName() == modelName)
            return item;
    }

    return NULL;
}

template< class ItemT, class InheritedT >
void CT_AbstractGroupModelT<ItemT,InheritedT>::clearGroups()
{
    while(!_groups.isEmpty())
    {
        CT_AbstractGroupModelT<ItemT,InheritedT> *group = _groups.takeFirst();
        groupRemoved(group);

        delete group;
    }
}

template< class ItemT, class InheritedT >
void CT_AbstractGroupModelT<ItemT,InheritedT>::clearItems()
{
    while(!_items.isEmpty())
    {
        ItemT *item = _items.takeFirst();
        itemRemoved(item);

        delete item;
    }
}

template< class ItemT, class InheritedT >
void CT_AbstractGroupModelT<ItemT,InheritedT>::clear()
{
    clearGroups();
    clearItems();
}

template< class ItemT, class InheritedT >
QList<CT_AbstractModel*> CT_AbstractGroupModelT<ItemT,InheritedT>::childrens() const
{
    QList<CT_AbstractModel*> l;

    QListIterator<CT_AbstractGroupModelT<ItemT,InheritedT>*> it(groups());

    while(it.hasNext())
        l.append(it.next());

    QListIterator<ItemT*> iti(items());

    while(iti.hasNext())
        l.append(iti.next());

    return l;
}

template< class ItemT, class InheritedT >
bool CT_AbstractGroupModelT<ItemT,InheritedT>::isEmpty() const
{
    return (_groups.isEmpty() && _items.isEmpty());
}

template< class ItemT, class InheritedT >
void CT_AbstractGroupModelT<ItemT,InheritedT>::addGroupWithoutVerification(CT_AbstractGroupModelT<ItemT,InheritedT> *group)
{
    this->staticSetParentModelToModel(group, this);
    _groups.append(group);

    groupAdded(group);
}

template< class ItemT, class InheritedT >
void CT_AbstractGroupModelT<ItemT,InheritedT>::addItemWithoutVerification(ItemT *item)
{
    this->staticSetParentModelToModel(item, this);
    _items.append(item);

    itemAdded(item);
}

template< class ItemT, class InheritedT >
CT_AbstractGroupModelT<ItemT,InheritedT>* CT_AbstractGroupModelT<ItemT,InheritedT>::rootGroup() const
{
    CT_AbstractGroupModelT<ItemT,InheritedT> *parent = (CT_AbstractGroupModelT<ItemT,InheritedT>*)this;

    CT_AbstractGroupModelT<ItemT,InheritedT> *p = NULL;

    // on remonte tout en haut de l'arbre
    while((p = parent->parentGroup()) != NULL)
        parent = p;

    return parent;
}

#endif // CT_ABSTRACTGROUPMODELT_HPP
