#ifndef CT_ABSTRACTITEMATTRIBUTEMODELT_HPP
#define CT_ABSTRACTITEMATTRIBUTEMODELT_HPP

#include "ct_attributes/model/abstract/ct_abstractitemattributemodelt.h"

template <class ItemModelT, class InheritedT>
CT_AbstractItemAttributeModelT<ItemModelT, InheritedT>::CT_AbstractItemAttributeModelT(const QString &uniqueName,
                                                                                       const QString &description,
                                                                                       const QString &displayableName) : InheritedT(uniqueName,
                                                                                                                                    description,
                                                                                                                                    displayableName)
{
}

template <class ItemModelT, class InheritedT>
CT_AbstractItemAttributeModelT<ItemModelT, InheritedT>::~CT_AbstractItemAttributeModelT()
{
}

template <class ItemModelT, class InheritedT>
QList<CT_AbstractModel*> CT_AbstractItemAttributeModelT<ItemModelT, InheritedT>::childrens() const
{
    return QList<CT_AbstractModel*>();
}

template <class ItemModelT, class InheritedT>
bool CT_AbstractItemAttributeModelT<ItemModelT, InheritedT>::isEmpty() const
{
    return true;
}

#endif // CT_ABSTRACTITEMATTRIBUTEMODELT_HPP
