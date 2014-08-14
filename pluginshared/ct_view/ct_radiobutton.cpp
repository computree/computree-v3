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

#include "ct_radiobutton.h"

CT_RadioButton::CT_RadioButton(QString text, int &value, int excludeValue, QString description)
{
    _data._excludeValue = excludeValue;
    _data._value = &value;
    _data._text = text;
    _description = description;

    _rdCreated = NULL;
}

QString CT_RadioButton::type() const
{
    return "CT_RadioButton";
}

QList<SettingsNodeGroup*> CT_RadioButton::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    values->addValue(new SettingsNodeValue("Version", "1"));
    values->addValue(new SettingsNodeValue("CheckedIndex", getValue(), _description));
    values->addValue(new SettingsNodeValue("Text", _data._text));

    return retList;
}

bool CT_RadioButton::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("CheckedIndex");

    if(values.isEmpty())
        return false;

    return setWidgetValue(values.first()->value());
}

QWidget* CT_RadioButton::createWidget(QWidget &parent)
{
    if(_rdCreated == NULL)
    {
        _rdCreated = new QRadioButton(_data._text, &parent);
        _rdCreated->setChecked((*(_data._value)) == _data._excludeValue);
    }

    return _rdCreated;
}

void CT_RadioButton::updateValue()
{
    if(_rdCreated->isChecked())
    {
        *(_data._value) = _data._excludeValue;
    }
}

bool CT_RadioButton::isValueAndWidgetValueDifferent() const
{
    return ((*_data._value) != _data._excludeValue);
}

QVariant CT_RadioButton::getValue() const
{
    return QVariant((*_data._value));
}

bool CT_RadioButton::setWidgetValue(QVariant val)
{
    bool ok;

    int value = val.toInt(&ok);

    if(ok)
    {
        _rdCreated->setChecked(value == _data._excludeValue);
    }

    return ok;
}

int CT_RadioButton::getId() const
{
    return _data._excludeValue;
}
