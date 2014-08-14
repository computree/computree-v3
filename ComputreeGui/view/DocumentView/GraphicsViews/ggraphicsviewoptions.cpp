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


#include "ggraphicsviewoptions.h"
#include "ui_ggraphicsviewoptions.h"

#include <QMessageBox>

GGraphicsViewOptions::GGraphicsViewOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGraphicsViewOptions)
{
    ui->setupUi(this);

    _options = new DM_GraphicsViewOptions();

    _colorSelected = new QtColorPicker(this);
    _colorSelected->setStandardColors();

    _colorBackground = new QtColorPicker(this);
    _colorBackground->setStandardColors();

    ui->horizontalLayoutSelectedColor->addWidget(_colorSelected);
    ui->horizontalLayoutBackgroundColor->addWidget(_colorBackground);

    updateUiFromOptions();

    connect(_colorSelected, SIGNAL(colorChanged(QColor)), this, SLOT(selectedColor(QColor)));
    connect(_colorBackground, SIGNAL(colorChanged(QColor)), this, SLOT(backgroundColor(QColor)));
    connect(ui->doubleSpinBoxPointSize, SIGNAL(valueChanged(double)), this, SLOT(pointSize(double)));
    connect(ui->checkBoxDrawAxis, SIGNAL(clicked(bool)), this, SLOT(drawAxis(bool)));
    connect(ui->checkBoxUseTransparency, SIGNAL(clicked(bool)), this, SLOT(useTransparency(bool)));
    connect(ui->checkBoxUseLight, SIGNAL(clicked(bool)), this, SLOT(useLight(bool)));
    connect(ui->spinBoxFastDrawTime, SIGNAL(valueChanged(int)), this, SLOT(fastDrawTime(int)));

    connect(ui->buttonGroupFastest, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(drawFastestMode(QAbstractButton*)));

    connect(ui->checkBoxCameraPosition, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->checkBoxCameraCenterCoordinates, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->checkBoxCameraViewDirection, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->buttonGroupCameraInfoPosition, SIGNAL(buttonClicked(QAbstractButton*)), this , SLOT(setCameraInfoPosition(QAbstractButton*)));
    connect(ui->buttonGroupViewType, SIGNAL(buttonClicked(QAbstractButton*)), this , SLOT(setCameraType(QAbstractButton*)));

    connect(ui->pushButtonSaveDefault, SIGNAL(clicked()), this, SLOT(saveDefault()));

    //setFixedSize(size());
}

GGraphicsViewOptions::~GGraphicsViewOptions()
{
    delete ui;
    delete _options;
}

void GGraphicsViewOptions::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GGraphicsViewOptions::setOptions(const DM_GraphicsViewOptions &options)
{
    _options->updateFromOtherOptions(options);
    
    updateUiFromOptions();
}

const DM_GraphicsViewOptions& GGraphicsViewOptions::getOptions()
{
    return *_options;
}

void GGraphicsViewOptions::updateUiFromOptions()
{
    _colorSelected->setCurrentColor(_options->getSelectedColor());
    _colorBackground->setCurrentColor(_options->getBackgroundColor());

    ui->doubleSpinBoxPointSize->setValue(_options->getPointSize());
    ui->checkBoxDrawAxis->setChecked(_options->drawAxis());
    ui->checkBoxUseTransparency->setChecked(_options->useTransparency());
    ui->checkBoxUseLight->setChecked(_options->useLight());
    ui->spinBoxFastDrawTime->setValue(_options->getFastDrawTime());
    ui->checkBoxCameraPosition->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraPosition));
    ui->checkBoxCameraCenterCoordinates->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraSceneCenter));
    ui->checkBoxCameraViewDirection->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraViewDirection));

    switch(_options->drawFastest())
    {
    case DM_GraphicsViewOptions::Always : ui->radioButtonFastestAlways->setChecked(true);
        break;

    case DM_GraphicsViewOptions::Normal : ui->radioButtonFastestNormal->setChecked(true);
        break;

    case DM_GraphicsViewOptions::Never : ui->radioButtonFastestNever->setChecked(true);
    }

    switch(_options->getCameraInformationPosition())
    {
    case DM_GraphicsViewOptions::InfoInUpperLeftCorner : ui->radioButtonUpperLeft->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInUpperRightCorner : ui->radioButtonUpperRight->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInLowerRightCorner : ui->radioButtonLowerRight->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInLowerLeftCorner : ui->radioButtonLowerLeft->setChecked(true);
    }

    switch(_options->getCameraType())
    {
    case CameraInterface::PERSPECTIVE : ui->radioButtonPerspective->setChecked(true);
        break;
    case CameraInterface::ORTHOGRAPHIC : ui->radioButtonOrthographic->setChecked(true);
        break;
    }
}

void GGraphicsViewOptions::backgroundColor(QColor color)
{
    _options->setBackgroudColor(color);
}

void GGraphicsViewOptions::selectedColor(QColor color)
{
    _options->setSelectedColor(color);
}

void GGraphicsViewOptions::pointSize(double size)
{
    _options->setPointSize(size);
    emit pointSizeChanged(size);
}

void GGraphicsViewOptions::drawAxis(bool e)
{
    _options->drawAxis(e);
}

void GGraphicsViewOptions::drawFastestMode(QAbstractButton* button)
{
    DM_GraphicsViewOptions::DrawFastestMode mode = DM_GraphicsViewOptions::Normal;

    if(button == ui->radioButtonFastestAlways)
    {
        mode = DM_GraphicsViewOptions::Always;
    }
    else if(button == ui->radioButtonFastestNever)
    {
        mode = DM_GraphicsViewOptions::Never;
    }

    _options->drawFastest(mode);
    emit drawModeChanged(mode);
}


void GGraphicsViewOptions::useTransparency(bool e)
{
    _options->useTransparency(e);
}

void GGraphicsViewOptions::useLight(bool e)
{
    _options->useLight(e);
}

void GGraphicsViewOptions::fastDrawTime(int t)
{
    _options->setFastDrawTime(t);
}

void GGraphicsViewOptions::setCameraInfoDisplayed()
{
    DM_GraphicsViewOptions::CameraInfoDisplayed info = DM_GraphicsViewOptions::CameraNone;

    if(ui->checkBoxCameraPosition->isChecked())
        info |= DM_GraphicsViewOptions::CameraPosition;

    if(ui->checkBoxCameraCenterCoordinates->isChecked())
        info |= DM_GraphicsViewOptions::CameraSceneCenter;

    if(ui->checkBoxCameraViewDirection->isChecked())
        info |= DM_GraphicsViewOptions::CameraViewDirection;

    _options->setCameraInformationDisplayed(info);
}

void GGraphicsViewOptions::setCameraInfoPosition(QAbstractButton* button)
{
    DM_GraphicsViewOptions::CameraInfoPosition pos = DM_GraphicsViewOptions::InfoInLowerRightCorner;

    if(button == ui->radioButtonUpperLeft)
    {
        pos = DM_GraphicsViewOptions::InfoInUpperLeftCorner;
    }
    else if(button == ui->radioButtonUpperRight)
    {
        pos = DM_GraphicsViewOptions::InfoInUpperRightCorner;
    }

    else if(button == ui->radioButtonLowerLeft)
    {
        pos = DM_GraphicsViewOptions::InfoInLowerLeftCorner;
    }

    _options->setCameraInformationPosition(pos);
}

void GGraphicsViewOptions::setCameraType(QAbstractButton *button)
{
    _options->setCameraType((button == ui->radioButtonPerspective ? CameraInterface::PERSPECTIVE : CameraInterface::ORTHOGRAPHIC));
}

void GGraphicsViewOptions::saveDefault()
{
    if(_options->save())
    {
        QMessageBox::information(this, trUtf8("Sauvegarde r\xc3\xa9""ussi"), trUtf8("La sauvegarde de la configuration a r\xc3\xa9""ussi."));
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur lors de la sauvegarde"), trUtf8("La sauvegarde de la configuration a \xc3\xa9""chou\xc3\xa9""."));
    }
}
