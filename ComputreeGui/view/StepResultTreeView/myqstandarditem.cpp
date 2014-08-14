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


#include "myqstandarditem.h"

#include "interfaces.h"

MyQStandardItem::MyQStandardItem(Step *step,
                                Result *res,
                                MyQStandardItem::ColumnType type,
                                const QString &text) : QStandardItem(text)
{
    _step = step;
    _result = res;
    _itemDrawable = NULL;
    _columnType = type;

    if(_step != NULL)
        connect(_step->getStepSignalEmmiter(), SIGNAL(destroyed()), this, SLOT(stepDestroyed()), Qt::DirectConnection);

    if(_result != NULL)
        connect(_result, SIGNAL(destroyed()), this, SLOT(itemDrawableDestroyed()), Qt::DirectConnection);

    setData(type, ColumnTypeRole);
}

MyQStandardItem::MyQStandardItem(ItemDrawable *id,
                                MyQStandardItem::ColumnType type,
                                const QString &text) : QStandardItem(text)
{
    _step = NULL;
    _result = NULL;
    _itemDrawable = id;
    _columnType = type;

    if(_itemDrawable != NULL)
        connect(_itemDrawable->getItemDrawableSignalSlotManager(), SIGNAL(destroyed()), this, SLOT(itemDrawableDestroyed()), Qt::DirectConnection);

    setData(type, ColumnTypeRole);
}

MyQStandardItem::MyQStandardItem(Step *step,
                                Result *res,
                                MyQStandardItem::ColumnType type,
                                const QVariant &value)
{
    _step = step;
    _result = res;
    _itemDrawable = NULL;
    _columnType = type;
    setData(value, Qt::DisplayRole);

    if(_step != NULL)
        connect(_step->getStepSignalEmmiter(), SIGNAL(destroyed()), this, SLOT(stepDestroyed()), Qt::DirectConnection);

    if(_result != NULL)
        connect(_result, SIGNAL(destroyed()), this, SLOT(itemDrawableDestroyed()), Qt::DirectConnection);

    setData(type, ColumnTypeRole);
}

MyQStandardItem::MyQStandardItem(ItemDrawable *id,
                                MyQStandardItem::ColumnType type,
                                const QVariant &value)
{
    _step = NULL;
    _result = NULL;
    _itemDrawable = id;
    _columnType = type;
    setData(value, Qt::DisplayRole);

    setData(type, ColumnTypeRole);
}

void MyQStandardItem::setData(const QVariant &value, int role)
{
    QStandardItem::setData(value, role);

    if((role == Qt::DisplayRole)
        || (role == Qt::EditRole)
        || (role == Qt::CheckStateRole)
        || (role == InvisibleBoolTypeRole))
    {
        emit dataChanged(this);

        if(role == Qt::CheckStateRole)
        {
            emit checkStateChanged(checkState());
        }
    }
}

void MyQStandardItem::setDataWithoutSignal(const QVariant &value, int role)
{
    QStandardItem::setData(value, role);
}

void MyQStandardItem::setStringData(QString data, Qt::ItemDataRole role)
{
    setDataWithoutSignal(data, role);
}

void MyQStandardItem::setIntData(int data, Qt::ItemDataRole role)
{
    setDataWithoutSignal(data, role);
}

void MyQStandardItem::setFloatData(float data, Qt::ItemDataRole role)
{
    setDataWithoutSignal(data, role);
}

void MyQStandardItem::setBoolData(bool data, Qt::ItemDataRole role)
{
    setDataWithoutSignal(data ? Qt::Checked : Qt::Unchecked, role);
}

void MyQStandardItem::setBoolDataInvisible(bool data)
{
    setDataWithoutSignal(data, InvisibleBoolTypeRole);
}

void MyQStandardItem::setColorData(QColor data, Qt::ItemDataRole role)
{
    setData(data, role);
}

void MyQStandardItem::slotSetEnabled(bool enable)
{
    setEnabled(enable);
}

void MyQStandardItem::slotSetDisabled(bool disable)
{
    setEnabled(!disable);
}

void MyQStandardItem::setBoolDataToFalseWhenDataIsTrue(bool data, Qt::ItemDataRole role)
{
    if(data)
        setBoolData(false, role);
}

void MyQStandardItem::stepDestroyed()
{
    _step = NULL;
}

void MyQStandardItem::itemDrawableDestroyed()
{
    emit itemDrawableToBeDestroyed();

    _itemDrawable = NULL;
}

void MyQStandardItem::resultDestroyed()
{
    _result = NULL;
}
