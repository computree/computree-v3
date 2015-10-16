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

#include "ct_asciifilechoicebutton.h"
#include "qfiledialog.h"

#include <QDebug>

CT_AsciiFileChoiceButton::CT_AsciiFileChoiceButton(QString btlab,
                                                   QString fileFilter,
                                                   bool autoDetect,
                                                   const QStringList &neededFields,
                                                   QString &fileName,
                                                   bool &header,
                                                   QString &separator,
                                                   QString &decimal,
                                                   QLocale &locale,
                                                   int & skip,
                                                   QMap<QString, int> &columns,
                                                   QString description)
{
    _buttonLabel = btlab;
    _fileFilter = fileFilter;
    _autoDetect = autoDetect;
    _columnsAsString = "";

    _fileName = &fileName;
    _header = &header;
    _separator = &separator;
    _decimal = &decimal;
    _locale = &locale;
    _skip = &skip;
    _columns = &columns;

    _description = description;

    _widgetCreated = NULL;
    _scrollArea = NULL;
    _verticalLayout = NULL;
    _pushButtonCreated = NULL;
    _labelCreated = NULL;

    _dialog = new CT_TextFileConfigurationDialog(neededFields, NULL, *_fileName, _autoDetect);
    _dialog->setFileExtensionAccepted(QList<FileFormat>() << FileFormat(fileFilter, ""));
}

CT_AsciiFileChoiceButton::CT_AsciiFileChoiceButton(QString btlab,
                                                   QString fileFilter,
                                                   bool autoDetect,
                                                   QList<CT_TextFileConfigurationFields> &neededFields,
                                                   QString &fileName,
                                                   bool &header,
                                                   QString &separator,
                                                   QString &decimal,
                                                   QLocale &locale,
                                                   int & skip,
                                                   QMap<QString, int> &columns,
                                                   QString description)
{
    _buttonLabel = btlab;
    _fileFilter = fileFilter;
    _autoDetect = autoDetect;
    _columnsAsString = "";

    _fileName = &fileName;
    _header = &header;
    _separator = &separator;
    _decimal = &decimal;
    _locale = &locale;
    _skip = &skip;
    _columns = &columns;

    _description = description;

    _widgetCreated = NULL;
    _scrollArea = NULL;
    _verticalLayout = NULL;
    _pushButtonCreated = NULL;
    _labelCreated = NULL;

    _dialog = new CT_TextFileConfigurationDialog(neededFields, NULL, *_fileName, _autoDetect);
    _dialog->setFileExtensionAccepted(QList<FileFormat>() << FileFormat(fileFilter, ""));
}

CT_AsciiFileChoiceButton::~CT_AsciiFileChoiceButton()
{
    delete _dialog;
}

QString CT_AsciiFileChoiceButton::type() const
{
    return "CT_AsciiFileChoiceButton";
}

QList<SettingsNodeGroup*> CT_AsciiFileChoiceButton::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *values = new SettingsNodeGroup("Values");
    retList.append(values);

    values->addValue(new SettingsNodeValue("Version", "1"));
    values->addValue(new SettingsNodeValue("FileName", QVariant(*_fileName), ""));
    values->addValue(new SettingsNodeValue("HeaderPresent", QVariant(*_header), ""));
    values->addValue(new SettingsNodeValue("Separator", QVariant(*_separator), ""));
    values->addValue(new SettingsNodeValue("DecimalSeparator", QVariant(*_decimal), ""));
    values->addValue(new SettingsNodeValue("NbLinesToSkip", QVariant(*_skip), ""));
    values->addValue(new SettingsNodeValue("Columns", _columnsAsString, ""));

    return retList;
}

bool CT_AsciiFileChoiceButton::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    bool okSkip = true;

    QList<SettingsNodeValue*> values = list.first()->valuesByTagName("FileName");
    if(values.isEmpty()) {return false;}
    *_fileName = values.first()->value().toString();

    values.clear();
    values = list.first()->valuesByTagName("HeaderPresent");
    if(values.isEmpty()) {return false;}
    *_header = values.first()->value().toBool();


    values.clear();
    values = list.first()->valuesByTagName("Separator");
    if(values.isEmpty()) {return false;}
    *_separator = values.first()->value().toString();


    values.clear();
    values = list.first()->valuesByTagName("DecimalSeparator");
    if(values.isEmpty()) {return false;}
    *_decimal = values.first()->value().toString();

    if (*_decimal == ",")
    {
        *_locale = QLocale(QLocale::French, QLocale::France).name();
    } else {
        *_locale = QLocale(QLocale::English, QLocale::UnitedKingdom).name();
    }

    values.clear();
    values = list.first()->valuesByTagName("NbLinesToSkip");
    if(values.isEmpty()) {return false;}
    *_skip = values.first()->value().toInt(&okSkip);

    values.clear();
    values = list.first()->valuesByTagName("Columns");
    if(values.isEmpty()) {return false;}
    _columnsAsString = values.first()->value().toString();

    *_columns = _dialog->setFieldColumnsSelectedFromString(_columnsAsString);

    _labelCreated->setText(*_fileName);
    return okSkip;
}

QWidget* CT_AsciiFileChoiceButton::createWidget(QWidget &parent)
{
    if(_pushButtonCreated == NULL)
    {
        _widgetCreated = new QWidget(&parent);
        _pushButtonCreated = new QPushButton(_widgetCreated);
        _pushButtonCreated->setText(_buttonLabel);

        _scrollArea = new QScrollArea(_widgetCreated);
        _scrollArea->setFrameShape(QScrollArea::NoFrame);
        _scrollArea->setWidgetResizable(true);
        _scrollArea->setSizeAdjustPolicy(QScrollArea::AdjustToContents);

        _labelCreated = new QLabel(_scrollArea);

        _labelCreated->setText(*_fileName);

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

void CT_AsciiFileChoiceButton::updateValue()
{       
    *_fileName = _labelCreated->text();
}

bool CT_AsciiFileChoiceButton::isValueAndWidgetValueDifferent() const
{
    return (*_fileName != _labelCreated->text());
}

QVariant CT_AsciiFileChoiceButton::getValue() const
{
    return QVariant(*_fileName);
}

bool CT_AsciiFileChoiceButton::setWidgetValue(QVariant val)
{
    _labelCreated->setText(val.toString());

    return true;
}

void CT_AsciiFileChoiceButton::chooseFile()
{

    if (!QFile(*_fileName).exists())
    {
        QString s = QFileDialog::getOpenFileName(_widgetCreated, tr("Choisir un fichier"), "", _fileFilter);
        if (QFile(s).exists())
        {
            *_fileName = s;
            _labelCreated->setText(s);
        }   else {
            _labelCreated->setText("");
            *_fileName = "";
        }
    }

    _dialog->setHeader(*_header);
    _dialog->setSeparator(*_separator);
    _dialog->setDecimal(*_decimal);
    _dialog->setNLinesToSkip(*_skip);
    _dialog->setFileNameWithPath(*_fileName);
    _dialog->extractFieldsNames();

    if (!_columnsAsString.isEmpty())
    {
        _dialog->setFieldColumnsSelectedFromString(_columnsAsString);
    }

    if (_dialog->exec() == QDialog::Accepted)
    {
        *_fileName = _dialog->getFileNameWithPath();
        *_header = _dialog->hasHeader();
        *_separator = _dialog->getSeparator();
        *_decimal = QLocale(_dialog->getQLocaleName()).decimalPoint();
        *_locale = QLocale(_dialog->getQLocaleName());
        *_skip = _dialog->getNlinesToSkip();
        _columns->clear();
        *_columns = _dialog->getNeededFieldColumns();
        _columnsAsString = _dialog->getFieldColumnsSelectedAsString(*_columns);

        _labelCreated->setText(*_fileName);
    }

    emit fileChanged();
}

void CT_AsciiFileChoiceButton::setFormat(QString format)
{
        _fileFilter = format;
}

