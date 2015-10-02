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

#include "ct_stepconfigurabledialog.h"

#include "ct_spinbox.h"
#include "ct_doublespinbox.h"
#include "ct_checkbox.h"
#include "ct_filechoicebutton.h"
#include "ct_asciifilechoicebutton.h"
#include "ct_combobox.h"
#include "ct_radiobutton.h"
#include "ct_buttongroup.h"
#include "ct_lineedit.h"

#include <QLabel>

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLayoutItem>
#include <QMessageBox>

CT_StepConfigurableDialog::CT_StepConfigurableDialog()
{
    _dialog = new QDialog();

    QVBoxLayout *lay = new QVBoxLayout();

    _dialog->setLayout(lay);

    _wid = new QWidget(_dialog);

    lay->addWidget(_wid);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel, Qt::Horizontal, _dialog);

    buttonBox->button(QDialogButtonBox::Ok)->setFocus(Qt::OtherFocusReason);

    QObject::connect(buttonBox, SIGNAL(accepted()), _dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), _dialog, SLOT(reject()));

    lay->addWidget(buttonBox);

    _wid->setLayout(new QGridLayout());

    _step = NULL;
    _canEdit = true;
    _nRow = 0;

    setWindowTitle(QObject::tr("Configuration"));
}

CT_StepConfigurableDialog::~CT_StepConfigurableDialog()
{
    delete _dialog;

    qDeleteAll(_listWidgetWithValueReference.begin(), _listWidgetWithValueReference.end());
}

void CT_StepConfigurableDialog::setWindowTitle(QString title)
{
    _dialog->setWindowTitle(title);
}

QString CT_StepConfigurableDialog::windowTitle() const
{
    return _dialog->windowTitle();
}

int CT_StepConfigurableDialog::exec()
{
    endEdit();

    emit openned();

    int ret = _dialog->exec();

    if(ret == 1)
    {
        QListIterator<CT_WidgetWithValueReferenceInterface*> it(_listWidgetWithValueReference);

        while(it.hasNext())
        {
            it.next()->updateValue();
        }
    }

    return ret;
}

bool CT_StepConfigurableDialog::setStep(CT_VirtualAbstractStep &step)
{
    if(_step == NULL)
    {
        _step = &step;
        return true;
    }

    return false;
}

bool CT_StepConfigurableDialog::addText(QString firstColumnLabelText,
                                        QString secondColumnLabelText,
                                        QString thirdColumnLabelText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, firstColumnLabelText);
        addLabel(_nRow, 1, secondColumnLabelText);
        addLabel(_nRow, 2, thirdColumnLabelText);

        ++_nRow;

        return true;
    }

    return false;
}

bool CT_StepConfigurableDialog::addTitle(QString titleText)
{
    if(_canEdit)
    {
        getLayout(_wid)->addWidget(new QLabel(titleText, _wid), _nRow, 0, 1, 3);
        ++_nRow;

        return true;
    }

    return false;
}
bool CT_StepConfigurableDialog::addInt(QString beforeLabelText,
                                       QString afterLabelText,
                                       int minValue,
                                       int maxValue,
                                       int &value,
                                       QString helpText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, beforeLabelText);

        CT_SpinBox *spinBox = new CT_SpinBox(minValue, maxValue, value, beforeLabelText);

        _listWidgetWithValueReference.append(spinBox);

        addWidget(_nRow, 1, spinBox->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        return true;
    }

    return false;
}

bool CT_StepConfigurableDialog::addDouble(QString beforeLabelText,
                                          QString afterLabelText,
                                          double minValue,
                                          double maxValue,
                                          int nDecimals,
                                          double &value,
                                          double multValue,
                                          QString helpText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, beforeLabelText);

        CT_DoubleSpinBox *spinBox = new CT_DoubleSpinBox(minValue, maxValue, nDecimals, value, multValue, beforeLabelText);

        _listWidgetWithValueReference.append(spinBox);

        addWidget(_nRow, 1, spinBox->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        return true;
    }

    return false;
}

bool CT_StepConfigurableDialog::addBool(QString beforeLabelText,
                                        QString afterLabelText,
                                        QString checkBoxText,
                                        bool &value,
                                        QString helpText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, beforeLabelText);

        CT_CheckBox *checkBox = new CT_CheckBox(checkBoxText, value, beforeLabelText);

        _listWidgetWithValueReference.append(checkBox);

        addWidget(_nRow, 1, checkBox->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        return true;
    }

    return false;
}

bool CT_StepConfigurableDialog::addString(QString beforeLabelText,
                                          QString afterLabelText,
                                          QString &value,
                                          QString helpText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, beforeLabelText);

        CT_LineEdit *lineEdit = new CT_LineEdit(value, beforeLabelText);

        _listWidgetWithValueReference.append(lineEdit);

        addWidget(_nRow, 1, lineEdit->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        return true;
    }

    return false;
}

CT_ComboBox* CT_StepConfigurableDialog::addStringChoice(QString beforeLabelText,
                                                        QString afterLabelText,
                                                        QStringList valuesList,
                                                        QString &value,
                                                        QString helpText)
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, beforeLabelText);

        CT_ComboBox *comboBox = new CT_ComboBox(valuesList, value, beforeLabelText);

        _listWidgetWithValueReference.append(comboBox);

        addWidget(_nRow, 1, comboBox->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        if (value != "") {comboBox->setWidgetValue(value);}
        return comboBox;
    }

    return NULL;
}

CT_FileChoiceButton *CT_StepConfigurableDialog::addFileChoice(QString btLabel,
                                                              CT_FileChoiceButton::NeededFileType filetype,
                                                              QString fileFilter,
                                                              QStringList &value,
                                                              QString helpText)
{
    if(_canEdit)
    {

        CT_FileChoiceButton *fileChoiceButton = new CT_FileChoiceButton(btLabel, filetype, fileFilter, value, btLabel);

        _listWidgetWithValueReference.append(fileChoiceButton);

        addWidget(_nRow, 0, fileChoiceButton->createWidget(*_wid), 1, -1);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        fileChoiceButton->setWidgetValue(value);
        return fileChoiceButton;
    }

    return NULL;
}

CT_AsciiFileChoiceButton *CT_StepConfigurableDialog::addAsciiFileChoice(QString btlab,
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
    if(_canEdit)
    {

        CT_AsciiFileChoiceButton *fileChoiceButton = new CT_AsciiFileChoiceButton(btlab,
                                                                                  fileFilter,
                                                                                  autoDetect,
                                                                                  neededFields,
                                                                                  fileName,
                                                                                  header,
                                                                                  separator,
                                                                                  decimal,
                                                                                  locale,
                                                                                  skip,
                                                                                  columns,
                                                                                  description);
        _listWidgetWithValueReference.append(fileChoiceButton);

        addWidget(_nRow, 0, fileChoiceButton->createWidget(*_wid), 1, -1);

        ++_nRow;

        fileChoiceButton->setWidgetValue(fileName);
        return fileChoiceButton;
    }

    return NULL;
}

CT_AsciiFileChoiceButton *CT_StepConfigurableDialog::addAsciiFileChoice(QString btlab,
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
    if(_canEdit)
    {

        CT_AsciiFileChoiceButton *fileChoiceButton = new CT_AsciiFileChoiceButton(btlab,
                                                                                  fileFilter,
                                                                                  autoDetect,
                                                                                  neededFields,
                                                                                  fileName,
                                                                                  header,
                                                                                  separator,
                                                                                  decimal,
                                                                                  locale,
                                                                                  skip,
                                                                                  columns,
                                                                                  description);
        _listWidgetWithValueReference.append(fileChoiceButton);

        addWidget(_nRow, 0, fileChoiceButton->createWidget(*_wid), 1, -1);

        ++_nRow;

        fileChoiceButton->setWidgetValue(fileName);
        return fileChoiceButton;
    }

    return NULL;
}


CT_ButtonGroup& CT_StepConfigurableDialog::addButtonGroup(int &value)
{
    CT_ButtonGroup *bg = new CT_ButtonGroup(*_dialog, value);
    _listWidgetWithValueReference.append(bg);

    return *bg;
}

bool CT_StepConfigurableDialog::addExcludeValue(QString beforeLabelText,
                                                QString afterLabelText,
                                                QString radioButtonText,
                                                CT_ButtonGroup &group,
                                                int id,
                                                QString helpText)
{
    if(_canEdit)
    {
        CT_RadioButton *radioButton = new CT_RadioButton(radioButtonText, group.getValueReference(), id);

        if(!group.addRadioButton(radioButton))
        {
            delete radioButton;

            return false;
        }

        addLabel(_nRow, 0, beforeLabelText);

        if(!_listWidgetWithValueReference.contains(&group))
        {
            _listWidgetWithValueReference.append(&group);
        }

        addWidget(_nRow, 1, radioButton->createWidget(*_wid));

        addLabel(_nRow, 2, afterLabelText);

        addHelpButton(_nRow, 3, helpText);

        ++_nRow;

        return true;
    }

    return false;
}

void CT_StepConfigurableDialog::addEmpty()
{
    if(_canEdit)
    {
        addLabel(_nRow, 0, "");

        ++_nRow;
    }
}

void CT_StepConfigurableDialog::endEdit()
{
    _canEdit = false;
}

QList<SettingsNodeGroup*> CT_StepConfigurableDialog::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *gr = new SettingsNodeGroup("Version");
    gr->addValue(new SettingsNodeValue("V", "1"));
    retList.append(gr);

    gr = new SettingsNodeGroup("Widget");
    retList.append(gr);

    QListIterator<CT_WidgetWithValueReferenceInterface*> it(_listWidgetWithValueReference);

    while(it.hasNext())
    {
        CT_WidgetWithValueReferenceInterface *wid = it.next();

        SettingsNodeGroup *type = new SettingsNodeGroup(wid->type());
        QList<SettingsNodeGroup*> groups = wid->getAllValues();

        while(!groups.isEmpty())
            type->addGroup(groups.takeFirst());

        gr->addGroup(type);
    }

    return retList;
}

bool CT_StepConfigurableDialog::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if((list.size() != 2)
            || (list.at(1)->name() != "Widget"))
        return false;


    const QList<SettingsNodeGroup*> &groups = list.at(1)->groups();

    if(groups.size() != _listWidgetWithValueReference.size())
        return false;

    QListIterator<CT_WidgetWithValueReferenceInterface*> it(_listWidgetWithValueReference);
    QListIterator<SettingsNodeGroup*> it2(groups);

    while(it.hasNext())
    {
        CT_WidgetWithValueReferenceInterface *wid = it.next();
        SettingsNodeGroup *type = it2.next();

        if(type->name() != wid->type())
            return false;

        if(!wid->setAllValues(type->groups()))
            return false;

        wid->updateValue();
    }

    return true;
}

bool CT_StepConfigurableDialog::isSettingsModified() const
{
    return true;
}

/////////////// PRIVATE ///////////

QGridLayout* CT_StepConfigurableDialog::getLayout(QWidget *wid) const
{
    return (QGridLayout*)wid->layout();
}

void CT_StepConfigurableDialog::addWidget(int row, int column, QWidget *wid, int rowSpan, int colSpan)
{
    getLayout(_wid)->addWidget(wid, row, column, rowSpan, colSpan);
}

void CT_StepConfigurableDialog::addLabel(int row, int column, QString text)
{
    getLayout(_wid)->addWidget(new QLabel(text, _wid), row, column);
}

void CT_StepConfigurableDialog::addHelpButton(int row, int column, QString helpText)
{
    if(!helpText.isEmpty()) {
        HelpButton *button = new HelpButton(_wid);
        button->setPixmap(QPixmap(":/Icones/Icones/help.png").scaledToHeight(25,Qt::SmoothTransformation));
        button->helpText = helpText;
        button->setToolTip(helpText);

        connect(button, SIGNAL(clicked(QString)), this, SLOT(showHelpMessage(QString)));

        addWidget(row, column, button);
    }
}

void CT_StepConfigurableDialog::showHelpMessage(QString helpText)
{
    QMessageBox::information(_wid, tr("Aide"), helpText);
}
