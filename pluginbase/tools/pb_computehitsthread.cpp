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

#include "pb_computehitsthread.h"

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_iterator/ct_pointiterator.h"

PB_ComputeHitsThread::PB_ComputeHitsThread(CT_Grid3D<int> *grilleHits,
                                           CT_Grid2DXY<int> *grdXY,
                                           CT_Grid2DXZ<int> *grdXZ,
                                           CT_Grid2DYZ<int> *grdYZ, CT_Profile<int> *proX, CT_Profile<int> *proY,
                                           CT_Profile<int>  *proZ,
                                           const CT_Scene *scene) : CT_MonitoredQThread()
{
    _grilleHits = grilleHits;
    _grdXY = grdXY;
    _grdXZ = grdXZ;
    _grdYZ = grdYZ;
    _proX = proX;
    _proY = proY;
    _proZ = proZ;
    _scene = scene;
}

void PB_ComputeHitsThread::run()
{
    CT_PointIterator itP(_scene->getPointCloudIndex());
    size_t n_points = itP.size();

    size_t progressStep = n_points / 20;
    size_t prog = 0;
    size_t i = 0;

    while(itP.hasNext())
    {
        const CT_Point &point = itP.next().currentPoint();
        size_t indice;

        if (_grilleHits->indexAtXYZ(point(0), point(1), point(2), indice))
        {
            // Hits Computing
            _grilleHits->addValueAtIndex(indice, 1);
            if (_grdXY != NULL) {_grdXY->addValueAtXY(point(0), point(1), 1);}
            if (_grdXZ != NULL) {_grdXZ->addValueAtXZ(point(0), point(2), 1);}
            if (_grdYZ != NULL) {_grdYZ->addValueAtYZ(point(1), point(2), 1);}
            if (_proX != NULL)  {_proX->addValueForXYZ(point(0), point(1), point(2), 1);}
            if (_proY != NULL)  {_proY->addValueForXYZ(point(0), point(1), point(2), 1);}
            if (_proZ != NULL)  {_proZ->addValueForXYZ(point(0), point(1), point(2), 1);}
        } else {
            qDebug() << "Le point "<< i << " de la scene n'est pas dans la grille";
        }

        ++i;

        if (i % progressStep == 0)
        {
            prog = (((size_t)100)*i)/n_points;
            _progress = prog;
            emit progressChanged();
        }
    }

    _grilleHits->computeMinMax();
    if (_grdXY != NULL) {_grdXY->computeMinMax();}
    if (_grdXZ != NULL) {_grdXZ->computeMinMax();}
    if (_grdYZ != NULL) {_grdYZ->computeMinMax();}
    if (_proX != NULL) {_proX->computeMinMax();}
    if (_proY != NULL) {_proY->computeMinMax();}
    if (_proZ != NULL) {_proZ->computeMinMax();}

    _progress = 100;
    emit progressChanged();
}

