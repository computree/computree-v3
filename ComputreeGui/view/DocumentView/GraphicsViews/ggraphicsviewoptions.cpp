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

#include <QTimer>
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
    connect(ui->checkBoxDrawGrid, SIGNAL(clicked(bool)), this, SLOT(setDrawGrid(bool)));
    connect(ui->checkBoxUseTransparency, SIGNAL(clicked(bool)), this, SLOT(useTransparency(bool)));
    connect(ui->checkBoxUseLight, SIGNAL(clicked(bool)), this, SLOT(useLight(bool)));

    connect(ui->checkBoxCameraPosition, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->checkBoxCameraCenterCoordinates, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->checkBoxCameraViewDirection, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->checkBoxShowFPS, SIGNAL(clicked(bool)), this, SLOT(setCameraInfoDisplayed()));
    connect(ui->buttonGroupCameraInfoPosition, SIGNAL(buttonClicked(QAbstractButton*)), this , SLOT(setCameraInfoPosition(QAbstractButton*)));
    connect(ui->buttonGroupViewType, SIGNAL(buttonClicked(QAbstractButton*)), this , SLOT(setCameraType(QAbstractButton*)));

    connect(ui->pushButtonSaveDefault, SIGNAL(clicked()), this, SLOT(saveDefault()));

    connect(ui->groupBoxCameraCoordinates, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxColorAndSize, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxDrawing, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxView, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));

    QString style = QString("QGroupBox::indicator {"
                                "width: 12px;"
                                "height: 12px;"
                            "}"
                            "QGroupBox::indicator:unchecked {"
                                "image: url(:/Icones/Icones/stylesheet-branch-closed.png);"
                            "}"
                            "QGroupBox::indicator:checked {"
                                "image: url(:/Icones/Icones/stylesheet-branch-open.png);"
                            "}");

    ui->groupBoxCameraCoordinates->setStyleSheet(style);
    ui->groupBoxColorAndSize->setStyleSheet(style);
    ui->groupBoxDrawing->setStyleSheet(style);
    ui->groupBoxView->setStyleSheet(style);

    // disable visibility of this parameters because with OSG it will not be used
    ui->checkBoxUseLight->setVisible(false);
    ui->checkBoxUseTransparency->setVisible(false);
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
    ui->checkBoxDrawGrid->setChecked(_options->drawGrid());
    ui->checkBoxUseTransparency->setChecked(_options->useTransparency());
    ui->checkBoxUseLight->setChecked(_options->useLight());
    ui->checkBoxCameraPosition->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraPosition));
    ui->checkBoxCameraCenterCoordinates->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraSceneCenter));
    ui->checkBoxCameraViewDirection->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraViewDirection));
    ui->checkBoxShowFPS->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::FpsInformation));

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

void GGraphicsViewOptions::setDrawGrid(bool e)
{
    _options->setDrawGrid(e);
}

void GGraphicsViewOptions::useTransparency(bool e)
{
    _options->useTransparency(e);
}

void GGraphicsViewOptions::useLight(bool e)
{
    _options->useLight(e);
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

    if(ui->checkBoxShowFPS->isChecked())
        info |= DM_GraphicsViewOptions::FpsInformation;

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

void GGraphicsViewOptions::collapseOrExpandGroupBox(bool val)
{
   QGroupBox *box = (QGroupBox*)sender();

   box->setMaximumHeight(1000);

   if(!val)
       box->setMaximumHeight(25);
   else
       box->adjustSize();

   adjustSize();
   QTimer::singleShot(5, this, SLOT(refreshDialog()));
}

void GGraphicsViewOptions::refreshDialog()
{
    resize(width()+1, height());
}

void GGraphicsViewOptions::saveDefault()
{
    if(_options->save())
    {
        QMessageBox::information(this, tr("Sauvegarde réussie"), tr("La sauvegarde de la configuration a réussi."));
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur lors de la sauvegarde"), tr("La sauvegarde de la configuration a échoué."));
    }
}
