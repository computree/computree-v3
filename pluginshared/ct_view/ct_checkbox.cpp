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

#include "interfaces.h"

#include "ct_checkbox.h"

CT_CheckBox::CT_CheckBox(QString text, bool &value, QString description)
{
    _data._value = &value;
    _data._text = text;
    _description = description;

    _checkBoxCreated = NULL;
}

QString CT_CheckBox::type() const
{
    return "CT_CheckBox";
}

QList<SettingsNodeGroup*> CT_CheckBox::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    SettingsNodeValue *value = new SettingsNodeValue("Version", "1");
    values->addValue(value);

    values->addValue(new SettingsNodeValue("Checked", getValue(), _description));
    values->addValue(new SettingsNodeValue("Text", _data._text));

    return retList;
}

bool CT_CheckBox::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("Checked");

    if(values.isEmpty())
        return false;

    return setWidgetValue(values.first()->value());
}

QWidget* CT_CheckBox::createWidget(QWidget &parent)
{
    if(_checkBoxCreated == NULL)
    {
        _checkBoxCreated = new QCheckBox(_data._text, &parent);
        _checkBoxCreated->setChecked(*(_data._value));
    }

    return _checkBoxCreated;
}

void CT_CheckBox::updateValue()
{
    *(_data._value) = _checkBoxCreated->isChecked();
}

bool CT_CheckBox::isValueAndWidgetValueDifferent() const
{
    return ((*_data._value) != _checkBoxCreated->isChecked());
}

QVariant CT_CheckBox::getValue() const
{
    return QVariant((*_data._value));
}

bool CT_CheckBox::setWidgetValue(QVariant val)
{
    _checkBoxCreated->setChecked(val.toBool());

    return true;
}
