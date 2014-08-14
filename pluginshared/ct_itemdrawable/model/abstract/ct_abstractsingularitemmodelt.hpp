#ifndef CT_ABSTRACTITEMDRAWABLEMODELT_HPP
#define CT_ABSTRACTITEMDRAWABLEMODELT_HPP

#include "ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.h"

template <class GroupModelT, class AttributModelT, class InheritedT>
CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::CT_AbstractSingularItemModelT(const QString &uniqueName,
                                                                                                      const QString &description,
                                                                                                      const QString &displayableName) : InheritedT(uniqueName, description, displayableName)
{
}

template <class GroupModelT, class AttributModelT, class InheritedT>
CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::~CT_AbstractSingularItemModelT()
{
    clearItemAttributes();
}


template <class GroupModelT, class AttributModelT, class InheritedT>
bool CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::addItemAttribute(AttributModelT *attribute)
{
    if(this->internalExistModelOrChildrensInTree(this->rootModel(), attribute)
            || attribute->uniqueName().isEmpty())
    {
        delete attribute;

        return false;
    }

    internalAddItemAttribute(attribute);

    return true;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
bool CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::addItemAttribute(AttributModelT *attribute, CT_AutoRenameModels &autoName)
{
    CT_AbstractModel *root = this->rootModel();

    // si on a déjà attribué un nouveau nom à un attribut précédent
    if(!autoName.models().isEmpty())
    {
        // si l'ancien modèle n'est pas un attribut
        if(dynamic_cast<AttributModelT*>(autoName.models().first()) == NULL)
        {
            delete attribute;

            // Il faut un autre CT_AutoRenameModels pour pouvoir ajouter cet attribut
            return false;
        }

        // on attribue le nom déjà défini au nouvel attribut
        attribute->setUniqueName(autoName.completeName());

        // on vérifie qu'il n'existe pas déjà dans notre arborescence
        if(!this->internalExistModelOrChildrensInTree(root, attribute))
        {
            // si c'est bon on l'ajoute
            internalAddItemAttribute(attribute);
            autoName.addModel(attribute);
            return true;
        }
    }
    else // sinon
    {
        // on met un nom par défaut
        autoName.setName("i");
        autoName.setExtra(0);

        attribute->setUniqueName(autoName.completeName());
    }

    // tant que l'attribut existe avec ce nom dans l'arborescence
    // ou tant qu'on ne peut pas renommer les anciens attributs avec
    // ce nouveau nom
    while(this->internalExistModelOrChildrensInTree(root, attribute)
          || !autoName.renameAllModels())
    {
        // on renomme le nouvel attribut
        autoName.setExtra(autoName.extra()+1);
        attribute->setUniqueName(autoName.completeName());
    }

    // on ajoute ce modele à l'AutoRename
    autoName.addModel(attribute);

    // on ajoute ce modele à notre liste
    internalAddItemAttribute(attribute);

    return true;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
bool CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::removeItemAttribute(AttributModelT *attribute)
{
    if(m_attributes.removeOne(attribute))
    {
        itemAttributeRemoved(attribute);

        delete attribute;
        return true;
    }

    return false;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
const QList<AttributModelT*>& CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::itemAttributes() const
{
    return m_attributes;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
AttributModelT* CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::findItemAttribute(const QString &uniqueModelName) const
{
    QListIterator<AttributModelT*> it(itemAttributes());

    while(it.hasNext())
    {
        AttributModelT *att = it.next();

        if(att->uniqueName() == uniqueModelName)
            return att;
    }

    return NULL;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
void CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::clearItemAttributes()
{
    while(!m_attributes.isEmpty())
    {
        AttributModelT *attribute = m_attributes.takeFirst();
        itemAttributeRemoved(attribute);

        delete attribute;
    }
}

template <class GroupModelT, class AttributModelT, class InheritedT>
GroupModelT* CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::parentGroup() const
{
    return (GroupModelT*)this->parentModel();
}

template <class GroupModelT, class AttributModelT, class InheritedT>
QList<CT_AbstractModel*> CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::childrens() const
{
    QList<CT_AbstractModel*> l;

    QListIterator<AttributModelT*> it(itemAttributes());

    while(it.hasNext())
        l.append(it.next());

    return l;
}

template <class GroupModelT, class AttributModelT, class InheritedT>
bool CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::isEmpty() const
{
    return m_attributes.isEmpty();
}

template <class GroupModelT, class AttributModelT, class InheritedT>
void CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::internalAddItemAttribute(AttributModelT *attr)
{
    this->staticSetParentModelToModel(attr, this);
    m_attributes.append(attr);

    itemAttributeAdded(attr);
}

template <class GroupModelT, class AttributModelT, class InheritedT>
GroupModelT* CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT>::rootGroup() const
{
    GroupModelT *parent = parentGroup();

    if(parent == NULL)
        return NULL;

    return dynamic_cast< GroupModelT* >(parent->rootGroup());
}

#endif // CT_ABSTRACTITEMDRAWABLEMODELT_HPP
