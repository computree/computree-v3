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


#include "gmainprogressdialog.h"
#include <QCloseEvent>
#include <QVBoxLayout>

#include "dm_guimanager.h"

GMainProgressDialog::GMainProgressDialog(QWidget *parent, Qt::WindowFlags flags) : IMainProgressDialog(parent, flags)
{
    _canClose = false;
    m_canceled = false;

    m_data = NULL;

    setWindowTitle(tr("Progression"));

    _firstLabel = new QLabel(this);
    _firstProgressBar = new QProgressBar(this);

    _secondLabel = new QLabel("essai", this);
    _secondProgressBar = new QProgressBar(this);

    m_cancelButton = new QPushButton(tr("Annuler"), this);
    m_cancelButton->setVisible(false);

    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(setCanceled()), Qt::DirectConnection);

    QVBoxLayout *vLayout = new QVBoxLayout();

    vLayout->addWidget(_firstLabel);
    vLayout->addWidget(_firstProgressBar);
    vLayout->addWidget(_secondLabel);
    vLayout->addWidget(_secondProgressBar);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(m_cancelButton);

    vLayout->addLayout(hLayout);

    delete layout();
    setLayout(vLayout);

    setSecondLabelText("");
    setSecondValue(100);
}

bool GMainProgressDialog::isCanceled() const
{
    return m_canceled;
}

void GMainProgressDialog::setData(const void *data)
{
    m_data = (void*)data;
}

void* GMainProgressDialog::data() const
{
    return m_data;
}

void GMainProgressDialog::setCanceled(bool val)
{
    m_canceled = val;

    if(m_canceled)
        emit cancel();
}

void GMainProgressDialog::setCanClose(bool enable)
{
    _canClose = enable;
}

void GMainProgressDialog::enableCanClose()
{
    _canClose = true;
}

void GMainProgressDialog::disableCanClose()
{
    _canClose = false;
}

void GMainProgressDialog::setLabelText(QString text)
{
    _firstLabel->setText(text);
}

void GMainProgressDialog::setValue(int progress)
{
    _firstProgressBar->setValue(progress);
}

void GMainProgressDialog::setProgressRange(const int &min, const int &max)
{
    _firstProgressBar->setRange(min, max);
}

void GMainProgressDialog::setSecondLabelText(QString text)
{
    _secondLabel->setText(text);

    /*if(text.isEmpty())
    {
        layout()->removeWidget(_secondLabel);
        layout()->removeWidget(_secondProgressBar);

        _secondLabel->setVisible(false);
        _secondProgressBar->setVisible(false);
    }
    else
    {
        if(!layout()->children().contains(_secondLabel))
        {
            layout()->addWidget(_secondLabel);
            layout()->addWidget(_secondProgressBar);
        }
    }*/
}

void GMainProgressDialog::setSecondValue(int progress)
{
    _secondProgressBar->setValue(progress);
}

void GMainProgressDialog::enableCancelButton(bool enable)
{
    m_cancelButton->setVisible(enable);
}

void GMainProgressDialog::setCancelButtonText(const QString &text)
{
    m_cancelButton->setText(text);
}

void GMainProgressDialog::closeEvent(QCloseEvent *event)
{
    if(!_canClose)
        event->ignore();
    else
        event->accept();
}
