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


#include "gstepmanageroptions.h"
#include "ui_gstepmanageroptions.h"

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"

#include <QFileDialog>
#include <QMessageBox>

#include "qdebug.h"

GStepManagerOptions::GStepManagerOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GStepManagerOptions)
{
    ui->setupUi(this);

    ui->lineEditAutoSaveDirPath->setReadOnly(true);

    ui->cb_locale->addItem("fr_FR");
    ui->cb_locale->addItem("en_GB");

    ui->checkBoxForceInResultModelRecursivity->setChecked(CT_InAbstractResultModel::FORCE_RECURSIVITY);
}

GStepManagerOptions::~GStepManagerOptions()
{
    delete ui;
}

void GStepManagerOptions::setOptions(CDM_StepManagerOptions options)
{
    _options = options;

    ui->checkBoxAutoSave->setChecked(_options.isAutoSaveEnable());
    ui->lineEditAutoSaveDirPath->setText(_options.autoSaveDirPath());
    ui->checkBoxClearResultFromMemory->setChecked(_options.isAutoClearResultFromMemoryEnable());

    if (ui->cb_locale->findText(_options.getQLocale()) < 0)
    {
        ui->cb_locale->addItem(_options.getQLocale());
    }
    ui->cb_locale->setCurrentIndex(ui->cb_locale->findText(_options.getQLocale()));
}


CDM_StepManagerOptions GStepManagerOptions::getOptions()
{
    if(ui->checkBoxAutoSave->isChecked())
    {
        _options.enableAutoSave(ui->lineEditAutoSaveDirPath->text());
    }
    else
    {
        _options.disableAutoSave();
    }

    _options.setAutoClearResultFromMemory(ui->checkBoxClearResultFromMemory->isChecked());

    _options.setQLocale(ui->cb_locale->currentText());

    return _options;
}

/*bool GStepManagerOptions::checkAutoSaveDirectory(QString path)
{
    if(QFile::exists(path))
    {
        QStringList filter;
        filter << ".";
        filter << "..";

        QDir dir(path);

        if(dir.entryList(filter).isEmpty())
        {
            return true;
        }
    }

    return false;
}*/

void GStepManagerOptions::on_pushButtonAutoSave_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Dossier de sauvegarde"), _options.autoSaveDirPath());

    if(path.isNull() == false)
    {
        _options.enableAutoSave(path);
        setOptions(_options);
    }
}

void GStepManagerOptions::on_checkBoxForceInResultModelRecursivity_toggled(bool e)
{
    CT_InAbstractResultModel::FORCE_RECURSIVITY = e;
}

void GStepManagerOptions::on_cb_locale_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    emit localeChanged(ui->cb_locale->currentText());
}
