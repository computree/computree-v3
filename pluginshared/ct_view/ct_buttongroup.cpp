/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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

#include "ct_buttongroup.h"

#include "interfaces.h"

CT_ButtonGroup::CT_ButtonGroup(QWidget &parent, int &value)
{
    _data._parent = &parent;
    _data._value = &value;
    _bGCreated = new QButtonGroup();
}

CT_ButtonGroup::~CT_ButtonGroup()
{
    qDeleteAll(_data._rbList.begin(), _data._rbList.end());
}

QList<SettingsNodeGroup*> CT_ButtonGroup::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    values->addValue(new SettingsNodeValue("Version", "1"));
    values->addValue(new SettingsNodeValue("CheckedIndex", getValue()));

    return retList;
}

bool CT_ButtonGroup::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("CheckedIndex");

    if(values.isEmpty())
        return false;

    return setWidgetValue(values.first()->value());
}

QWidget* CT_ButtonGroup::createWidget(QWidget &parent)
{
    Q_UNUSED(parent)

    return NULL;
}

void CT_ButtonGroup::updateValue()
{
    *(_data._value) = _bGCreated->checkedId();
}

bool CT_ButtonGroup::isValueAndWidgetValueDifferent() const
{
    return ((*(_data._value)) != _bGCreated->checkedId());
}

QVariant CT_ButtonGroup::getValue() const
{
    return QVariant((*_data._value));
}

bool CT_ButtonGroup::setWidgetValue(QVariant val)
{
    bool ok;
    int id = val.toInt(&ok);

    if(ok)
    {
        ok = false;

        CT_RadioButton *button = _data._rbList.value(id, NULL);

        if(button != NULL)
        {
            ok = true;
            button->setWidgetValue(val);
        }
    }

    return ok;
}

bool CT_ButtonGroup::addRadioButton(CT_RadioButton *button)
{
    if((button != NULL)
            && (!_data._rbList.contains(button->getId())))
    {
        _bGCreated->addButton((QRadioButton*)button->createWidget(*_data._parent), button->getId());

        _data._rbList.insert(button->getId(), button);

        return true;
    }

    return false;
}

int& CT_ButtonGroup::getValueReference() const
{
    return *(_data._value);
}
