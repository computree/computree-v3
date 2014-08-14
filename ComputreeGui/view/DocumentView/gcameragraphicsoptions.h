/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GCAMERAGRAPHICSOPTIONS_H
#define GCAMERAGRAPHICSOPTIONS_H

#include <QWidget>

#include "dm_graphicsviewcamera.h"
#include "gcameracoordinatesoptions.h"
#include "dm_graphicsviewoptions.h"

namespace Ui {
    class GCameraGraphicsOptions;
}

class GCameraGraphicsOptions : public QWidget {
    Q_OBJECT
public:

    GCameraGraphicsOptions(QWidget *parent = 0);
    ~GCameraGraphicsOptions();

    void setCamera(const DM_GraphicsViewCamera *camera);
    DM_GraphicsViewCamera* getCamera() const { return _camera; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GCameraGraphicsOptions  *ui;
    DM_GraphicsViewCamera       *_camera;
    GCameraCoordinatesOptions   *_camCoordinateWidget;

    void setPushButtonSyncToolTip();

private slots:

    void on_pushButtonInitCenter_clicked();
    void on_pushButtonFixBarycenterSelected_clicked();
    void on_pushButtonFixBarycenter_clicked();

    void on_pushButtonSync_toggled(bool toggle);

    void on_pushButtonFitBB_clicked();

    void on_comboBoxPointOfView_activated(int index);

signals:
    void syncGraphics(bool enable);

};

#endif // GCAMERAGRAPHICSOPTIONS_H
