/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "dm_document.h"

int DM_Document::NUMBER = 1;

DM_Document::DM_Document(DM_DocumentManager &manager, QString title)
{
    _manager = &manager;
    _title = title;
    m_closeFilter = NULL;

    m_number = NUMBER;

    ++NUMBER;

    connect(this, SIGNAL(itemDrawableToBeRemoved(ItemDrawable&)), this, SLOT(slotItemToBeRemoved(ItemDrawable&)), Qt::DirectConnection);
    connect(this, SIGNAL(itemDrawableAdded(ItemDrawable&)), this, SLOT(slotItemDrawableAdded(ItemDrawable&)), Qt::DirectConnection);
}

void DM_Document::setDocumentCloseFilter(const DM_IDocumentCloseFilter *filter)
{
    m_closeFilter = (DM_IDocumentCloseFilter*)filter;
}

DM_IDocumentCloseFilter* DM_Document::closeFilter() const
{
    return m_closeFilter;
}

void DM_Document::setDocumentAddFilter(const DM_IDocumentAddFilter *filter)
{
    m_addFilter = (DM_IDocumentAddFilter*)filter;
}

DM_IDocumentAddFilter* DM_Document::addFilter() const
{
    return m_addFilter;
}

DM_DocumentManager* DM_Document::getManager() const
{
    return _manager;
}

int DM_Document::getNumber() const
{
    return m_number;
}

QString DM_Document::getTitle() const
{
    return _title;
}

bool DM_Document::canAddItemDrawable(const ItemDrawable *item) const
{
    if(m_addFilter != NULL)
        return m_addFilter->canAddItemDrawable(this, item);

    return true;
}

void DM_Document::addItemDrawable(ItemDrawable &item)
{
    if(!canAddItemDrawable(&item))
        return;

    if(item.addDocumentParent(this))
    {
        _listItemDrawable.append((ItemDrawable*)&item);

        emit itemDrawableAdded(item);
    }
}

void DM_Document::removeItemDrawable(ItemDrawable &item)
{
    if(_listItemDrawable.contains(&item))
    {
        emit itemDrawableToBeRemoved(item);

        item.removeDocumentParent(this);

        _listItemDrawable.removeOne(&item);
    }
}

void DM_Document::removeAllItemDrawableOfResult(const Result &res)
{
    Result *oResult = (Result*)&res;
    Result *result = (Result*)&res;

    if(res.getModel() != NULL)
    {
        IResultModel *om = res.getModel()->originalModel();

        if((om != NULL)
            && !om->getRootItemModel().isEmpty())
            oResult = om->getRootItemModel().first()->result();
    }

    beginRemoveMultipleItemDrawable();

    QMutableListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();
        Result *iResult = item->result();

        if((iResult == result) || (iResult == oResult))
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();
}

void DM_Document::removeAllItemDrawableOfModel(const IItemModel &model)
{
    Result *result = model.result();

    if(result == NULL)
        return;

    QString modelName = model.name();

    beginRemoveMultipleItemDrawable();

    QMutableListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if((item->result() == result)
                && (item->getModel()->name() == modelName))
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();
}

void DM_Document::removeAllSelectedItemDrawable()
{
    beginRemoveMultipleItemDrawable();

    QMutableListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->isSelected())
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();
}

void DM_Document::removeAllItemDrawable()
{
    beginRemoveMultipleItemDrawable();

    QMutableListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        emit itemDrawableToBeRemoved(*item);

        item->removeDocumentParent(this);
        it.remove();
    }

    endRemoveMultipleItemDrawable();
}

const QList<ItemDrawable*>& DM_Document::getItemDrawable() const
{
    return _listItemDrawable;
}

QList<ItemDrawable*> DM_Document::getSelectedItemDrawable() const
{
    QList<ItemDrawable*> list;

    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->isSelected())
        {
            list.append(item);
        }
    }

    return list;
}

size_t DM_Document::nItemDrawable() const
{
    return _listItemDrawable.size();
}

ItemDrawable* DM_Document::getItemDrawable(int i) const
{
    if((i>=0)
        && (i<_listItemDrawable.size()))
    {
        return _listItemDrawable.at(i);
    }

    return NULL;
}

void DM_Document::setSelectAllItemDrawable(bool select)
{
    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        it.next()->getItemDrawableSignalSlotManager()->select(select);
    }
}

void DM_Document::setSelectAllItemDrawableOfModel(bool select, const IItemModel &model)
{
    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->getModel() == &model)
            item->getItemDrawableSignalSlotManager()->select(select);
    }
}

QList<ItemDrawable*> DM_Document::findItemDrawable(const IItemModel &model) const
{
    QList<ItemDrawable*> list;

    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->getModel() == &model)
        {
            list.append(item);
        }
    }

    return list;
}

void DM_Document::findItemDrawable(const IItemModel &model, QList<ItemDrawable *> &outList) const
{
    outList.clear();

    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->getModel() == &model)
        {
            outList.append(item);
        }
    }
}

ItemDrawable* DM_Document::findFirstItemDrawable(const IItemModel &model) const
{
    QListIterator<ItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        if(item->getModel() == &model)
        {
            return item;
        }
    }

    return NULL;
}

void DM_Document::slotItemDrawableAdded(ItemDrawable &item)
{
    connect(item.getItemDrawableSignalSlotManager(), SIGNAL(selectChange(bool)), this, SLOT(slotItemDrawableSelectionChanged(bool)), Qt::DirectConnection);
}

void DM_Document::slotItemToBeRemoved(ItemDrawable &item)
{
    disconnect(item.getItemDrawableSignalSlotManager(), SIGNAL(selectChange(bool)), this, SLOT(slotItemDrawableSelectionChanged(bool)));
}

/////////// PRIVATE ////////////

void DM_Document::slotItemDrawableSelectionChanged(bool select)
{
    ItemDrawableSignalSlotManager *itemSSM = (ItemDrawableSignalSlotManager*)sender();
    ItemDrawable *item = itemSSM->getItemDrawable();

    emit itemDrawableSelectionChanged(item, select);
}
