/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#include "ct_combobox.h"

CT_ComboBox::CT_ComboBox(QStringList valuesList, QString &value, QString description)
{
    _data._value = &value;
    _data._valuesList = valuesList;
    _description = description;

    _comboBoxCreated = NULL;
}

QString CT_ComboBox::type() const
{
    return "CT_ComboBox";
}

QList<SettingsNodeGroup*> CT_ComboBox::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    values->addValue(new SettingsNodeValue("Version", "1"));
    values->addValue(new SettingsNodeValue("CurrentTextIndex", getValue(), _description));
    values->addValue(new SettingsNodeValue("ValueList", getStringListAsString(_data._valuesList), _description));

    return retList;
}

bool CT_ComboBox::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("CurrentTextIndex");
    QList<SettingsNodeValue*> valuesList = list.first()->valuesByTagName("ValueList");

    if(values.isEmpty())
        return false;

    if (valuesList.size() > 0)
    {
        setWidgetValueList(valuesList.first()->value());
    }

    bool val = false;
    if (values.size() >0) {val = setWidgetValue(values.first()->value());}

    return val;
}

QWidget* CT_ComboBox::createWidget(QWidget &parent)
{
    if(_comboBoxCreated == NULL)
    {
        _comboBoxCreated = new QComboBox(&parent);
        _comboBoxCreated->insertItems(0, _data._valuesList);
        int index = _comboBoxCreated->findText(*_data._value);
        if (index == -1) {index = 0;}
        _comboBoxCreated->setCurrentIndex(index);

    }

    connect(_comboBoxCreated, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(currentIndexChanged(QString)));

    return _comboBoxCreated;
}

void CT_ComboBox::updateValue()
{
    *(_data._value) = _comboBoxCreated->currentText();
}

bool CT_ComboBox::isValueAndWidgetValueDifferent() const
{
    return ((*_data._value) != _comboBoxCreated->currentText());
}

QVariant CT_ComboBox::getValue() const
{
    return QVariant((*_data._value));
}


bool CT_ComboBox::setWidgetValueList(QVariant valList)
{
    QStringList liste = valList.toString().split("\t");
    _comboBoxCreated->clear();
    _comboBoxCreated->addItems(liste);

    return true;
}

bool CT_ComboBox::setWidgetValue(QVariant val)
{
    _comboBoxCreated->setCurrentIndex(_comboBoxCreated->findText(val.toString()));

    return true;
}

void CT_ComboBox::changeValues(QStringList valuesList, QString value)
{
    _data._valuesList = valuesList;

    if (_comboBoxCreated != NULL)
    {
        _comboBoxCreated->clear();
        _comboBoxCreated->insertItems(0, _data._valuesList);
        int index = _comboBoxCreated->findText(value);
        if (index == -1) {index = 0;}
        _comboBoxCreated->setCurrentIndex(index);
    }
    *(_data._value) = _comboBoxCreated->currentText();
}


QString CT_ComboBox::getStringListAsString(const QStringList &liste)
{
    QString result = "";

    for (int i = 0 ; i < liste.size() ; i++)
    {
        result.append(liste.at(i));
        if (i < (liste.size() - 1)) {result.append("\t");}
    }
    return result;
}

