/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_container.h"

const CT_StandardContainerDrawManager CT_Container::CONTAINER_DRAW_MANAGER;

CT_Container::CT_Container() : CT_AbstractItemDrawable()
{
    setBaseDrawManager(&CONTAINER_DRAW_MANAGER);
}

CT_Container::CT_Container(const CT_OutAbstractItemModel *model,
                           const CT_AbstractResult *result,
                           const QString &defaultItemDrawableType) : CT_AbstractItemDrawable(model, result)
{
    m_defaultItemDrawableType = defaultItemDrawableType;
    setBaseDrawManager(&CONTAINER_DRAW_MANAGER);
}

CT_Container::~CT_Container()
{
    if(isAutoDelete())
        qDeleteAll(_list.begin(), _list.end());
}

QString CT_Container::getType() const
{
    return staticGetType();
}

QString CT_Container::staticGetType()
{
    return CT_AbstractItemDrawable::staticGetType() + "/CT_Container";
}

bool CT_Container::addItemDrawable(CT_AbstractItemDrawable *id)
{
    /*if(id->getType() != m_defaultItemDrawableType)
        return false;*/

    _list.append(id);

    return true;
}

bool CT_Container::removeItemDrawable(CT_AbstractItemDrawable *item)
{
    if(_list.removeOne(item))
    {
        m_itemRemovedLater.removeOne(item);

        if(isAutoDelete())
            delete item;

        return true;
    }

    return false;
}

bool CT_Container::removeItemDrawableWithoutAutoDelete(CT_AbstractItemDrawable *item)
{
    if(_list.removeOne(item))
    {
        m_itemRemovedLater.removeOne(item);
        return true;
    }

    return false;
}

QList<CT_AbstractItemDrawable*>* CT_Container::getList() const
{
    return (QList<CT_AbstractItemDrawable*>*)&_list;
}

const QList<CT_AbstractItemDrawable*>& CT_Container::itemThatWillBeRemovedLater() const
{
    return m_itemRemovedLater;
}

void CT_Container::setItemDrawableBeRemovedLater(const CT_AbstractItemDrawable *item)
{
    m_itemRemovedLater.append((CT_AbstractItemDrawable*)item);
}

void CT_Container::updateCenter()
{
    int size = _list.size();

    if(size > 0)
    {
        Eigen::Vector3d c(0,0,0);

        QListIterator<CT_AbstractItemDrawable*> it(_list);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            c += item->getCenterCoordinate();
        }

        c /= (double)size;
        setCenterX(c(0));
        setCenterY(c(1));
        setCenterZ(c(2));
    }
}

int CT_Container::indexOf(const CT_AbstractItemDrawable *child) const
{
    return _list.indexOf(dynamic_cast<CT_AbstractItemDrawable*>((CT_AbstractItemDrawable*)child));
}

int CT_Container::getFastestIncrement() const
{
    return 64;
}

CT_AbstractItemDrawable* CT_Container::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_ResultCopyModeList::CopyMode copyMode = CT_ResultCopyModeList::CopyItemDrawableReference;

    if(!copyModeList.isEmpty())
        copyMode = copyModeList.takeFirst();

    CT_Container *container = new CT_Container(model, result, m_defaultItemDrawableType);
    container->setId(id());
    container->setAutoDelete(true);

    if(copyMode == CT_ResultCopyModeList::CopyItemDrawableCompletely)
    {
        int size = _list.size();

        for(int i=0; i<size; ++i)
            container->_list.append(_list.at(i)->copy(NULL, result, copyModeList));
    }
    else if(copyMode == CT_ResultCopyModeList::CopyItemDrawableReference)
    {
        container->_list.append(_list);
        container->setAutoDelete(false);
    }

    container->setAlternativeDrawManager(getAlternativeDrawManager());

    return container;
}

QList<CT_ItemDrawableConfiguration*> CT_Container::getDependantViewConfigurations()
{
    QList<CT_ItemDrawableConfiguration*> liste;

    if(!_list.isEmpty())
    {
        liste.append(_list.first()->getViewConfiguration());
        liste.append(_list.first()->getDependantViewConfigurations());
    }

    return liste;
}

void CT_Container::internalSetWillBeRemovedFromResult(const CT_AbstractResult *res)
{
    QListIterator<CT_AbstractItemDrawable*> it(_list);

    while(it.hasNext())
        CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(it.next(), res);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Container)
#endif
