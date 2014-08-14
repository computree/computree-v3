/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginONFENSAM library 2.0.

 PluginONFENSAM is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginONFENSAM is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginONFENSAM.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef PB_COMPUTEHITSTHREAD_H
#define PB_COMPUTEHITSTHREAD_H

#include "ct_tools/ct_monitoredqthread.h"

#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_itemdrawable/ct_grid2dxz.h"
#include "ct_itemdrawable/ct_grid2dyz.h"
#include "ct_itemdrawable/ct_profile.h"


#include <QThread>

/*!
 * \brief Computes the "hit" grid of a point cloud
 *
 * Each voxel of the resulting grid stores the number of hits inside itself
 *
 * \param _grilleHits Output grid for hits number
 * \param _scene point cloud to take into account while computing the grid
 */
class PB_ComputeHitsThread : public CT_MonitoredQThread
{
    Q_OBJECT
public:
    PB_ComputeHitsThread(CT_Grid3D<int> *grilleHits,
                         CT_Grid2DXY<int> *grdXY,
                         CT_Grid2DXZ<int> *grdXZ,
                         CT_Grid2DYZ<int> *grdYZ,
                         CT_Profile<int>  *proX,
                         CT_Profile<int>  *proY,
                         CT_Profile<int>  *proZ,
                         CT_Profile<int>  *proDiag,
                         const CT_Scene *scene);

    void run();

private:
    CT_Grid3D<int>*     _grilleHits;
    CT_Grid2DXY<int>*   _grdXY;
    CT_Grid2DXZ<int>*   _grdXZ;
    CT_Grid2DYZ<int>*   _grdYZ;
    CT_Profile<int>*    _proX;
    CT_Profile<int>*    _proY;
    CT_Profile<int>*    _proZ;
    CT_Profile<int>*    _proDiag;
    const CT_Scene*     _scene;
};

#endif // PB_COMPUTEHITSTHREAD_H
