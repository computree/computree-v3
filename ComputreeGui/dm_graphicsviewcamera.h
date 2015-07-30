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


#ifndef DM_GRAPHICSVIEWCAMERA_H
#define DM_GRAPHICSVIEWCAMERA_H

#include "interfaces.h"

class DM_GraphicsViewCamera : public CameraInterface
{
    Q_INTERFACES(CameraInterface)

public:
    DM_GraphicsViewCamera();

public slots:
    virtual void syncWithCamera(const DM_GraphicsViewCamera *cam) = 0;
    virtual void alignCameraToInvXAxis() = 0;
    virtual void alignCameraToInvYAxis() = 0;
    virtual void alignCameraToInvZAxis() = 0;

    virtual void setPointOfView(double cx, double cy, double cz, double distance, double rx, double ry, double rz, double rw, bool redrawView = true) = 0;
};

#endif // DM_GRAPHICSVIEWCAMERA_H
