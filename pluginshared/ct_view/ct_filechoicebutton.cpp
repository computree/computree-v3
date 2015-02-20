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

#include "ct_filechoicebutton.h"
#include "qfiledialog.h"

CT_FileChoiceButton::CT_FileChoiceButton(QString btlab, CT_FileChoiceButton::NeededFileType filetype, QString fileFilter, QStringList &value, QString description)
{
    _data._buttonLabel = btlab;
    _data._fileType = filetype;
    _data._fileFilter = fileFilter;
    _data._value = &value;
    _description = description;

    _widgetCreated = NULL;
    _scrollArea = NULL;
    _verticalLayout = NULL;
    _pushButtonCreated = NULL;
    _labelCreated = NULL;
}

QString CT_FileChoiceButton::type() const
{
    return "CT_FileChoiceButton";
}

QList<SettingsNodeGroup*> CT_FileChoiceButton::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    values->addValue(new SettingsNodeValue("Version", "1"));
    values->addValue(new SettingsNodeValue("LabelValue", getValueAsString(), _description));

    return retList;
}

bool CT_FileChoiceButton::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("LabelValue");

    if(values.isEmpty())
        return false;

    return setWidgetValueAsString(values.first()->value());
}

QWidget* CT_FileChoiceButton::createWidget(QWidget &parent)
{
    if(_pushButtonCreated == NULL)
    {
        _widgetCreated = new QWidget(&parent);
        _pushButtonCreated = new QPushButton(_widgetCreated);
        _pushButtonCreated->setText(_data._buttonLabel);

        _scrollArea = new QScrollArea(_widgetCreated);
        _scrollArea->setFrameShape(QScrollArea::NoFrame);
        _scrollArea->setWidgetResizable(true);
        _scrollArea->setSizeAdjustPolicy(QScrollArea::AdjustToContents);

        _labelCreated = new QLabel(_scrollArea);

        _labelCreated->setText("");
        for (int i = 0 ; i < (*_data._value).size() ; ++i)
        {
            _labelCreated->setText(QString("%1\n%2").arg(_labelCreated->text()).arg((*_data._value).at(i)));
        }

        _verticalLayout = new QVBoxLayout(_widgetCreated);
        _verticalLayout->setSpacing(0);

        _verticalLayout->addWidget(_pushButtonCreated);
        _verticalLayout->addWidget(_scrollArea);
        _scrollArea->setWidget(_labelCreated);
        _labelCreated->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        connect(_pushButtonCreated, SIGNAL(clicked()),this,SLOT(chooseFile()));

    }

    return _widgetCreated;
}

void CT_FileChoiceButton::updateValue()
{       
    *(_data._value) = _labelCreated->text().split("\n", QString::SkipEmptyParts);
}

bool CT_FileChoiceButton::isValueAndWidgetValueDifferent() const
{
    return ((*_data._value) != _labelCreated->text().split("\n", QString::SkipEmptyParts));
}

QVariant CT_FileChoiceButton::getValue() const
{
    return QVariant((*_data._value));
}

QVariant CT_FileChoiceButton::getValueAsString() const
{
    return _labelCreated->text();
}

bool CT_FileChoiceButton::setWidgetValueAsString(QVariant val)
{
    QString value = val.toString();
    _labelCreated->setText(value);
    return true;
}

bool CT_FileChoiceButton::setWidgetValue(QVariant val)
{
    bool ok = true;

    QStringList value = val.toStringList();

    if(ok)
    {
        _labelCreated->setText("");
        for (int i = 0 ; i < value.size() ; ++i)
        {
            _labelCreated->setText(QString("%1\n%2").arg(_labelCreated->text()).arg(value.at(i)));
        }

    }

    return ok;
}

void CT_FileChoiceButton::chooseFile()
{
    _labelCreated->setText("");

    if (_data._fileType == CT_FileChoiceButton::OneExistingFile)
    {
        QString s = QFileDialog::getOpenFileName(_widgetCreated, tr("Choisir un fichier"), "", _data._fileFilter);
        if (QFile(s).exists())
        {
            _labelCreated->setText(s);
        }

    } else if (_data._fileType == CT_FileChoiceButton::OneOrMoreExistingFiles)
    {
        QStringList s = QFileDialog::getOpenFileNames(_widgetCreated, tr("Choisir un ou plusieurs fichiers"), "", _data._fileFilter);
        for (int i = 0 ; i < s.size() ; i++)
        {
            if (QFile(s.at(i)).exists())
            {
                _labelCreated->setText(QString("%1\n%2").arg(_labelCreated->text()).arg(s.at(i)));
            }
        }


    } else if (_data._fileType == CT_FileChoiceButton::OneExistingFolder)
    {
        QString s = QFileDialog::getExistingDirectory(_widgetCreated, tr("Choisir un répertoire"), "");
        if (QDir(s).exists())
        {
            _labelCreated->setText(s);
        }
    } else if (_data._fileType == CT_FileChoiceButton::OneNewFile)
    {
        QString s = QFileDialog::getSaveFileName(_widgetCreated, tr("Fichier à créer"), "", _data._fileFilter);
        _labelCreated->setText(s);
    }


}

void CT_FileChoiceButton::setFormat(QString format)
{
        _data._fileFilter = format;
}

