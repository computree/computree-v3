/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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

#include "ct_lineedit.h"

CT_LineEdit::CT_LineEdit(QString &value, QString description)
{
    _data._value = &value;
    _description = description;

    _lineEditCreated = NULL;
}

QString CT_LineEdit::type() const
{
    return "CT_LineEdit";
}

QList<SettingsNodeGroup*> CT_LineEdit::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    SettingsNodeValue *value = new SettingsNodeValue("Version", "1");
    values->addValue(value);

    values->addValue(new SettingsNodeValue("Value", getValue(), _description));

    return retList;
}

bool CT_LineEdit::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("Value");

    if(values.isEmpty())
        return false;

    return setWidgetValue(values.first()->value());
}

QWidget* CT_LineEdit::createWidget(QWidget &parent)
{
    if(_lineEditCreated == NULL)
    {
        _lineEditCreated = new QLineEdit(*(_data._value), &parent);
    }

    return _lineEditCreated;
}

void CT_LineEdit::updateValue()
{
    *(_data._value) = _lineEditCreated->text();
}

bool CT_LineEdit::isValueAndWidgetValueDifferent() const
{
    return ((*_data._value) != _lineEditCreated->text());
}

QVariant CT_LineEdit::getValue() const
{
    return QVariant((*_data._value));
}

bool CT_LineEdit::setWidgetValue(QVariant val)
{
    _lineEditCreated->setText(val.toString());

    return true;
}
