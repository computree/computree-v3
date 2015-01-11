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

#include "ct_planarbsplinedata.h"

#include <math.h>

CT_PlanarBSplineData::CT_PlanarBSplineData() : CT_ShapeData()
{
    _nCP = 0;
    _degree = 1;
}

CT_PlanarBSplineData::CT_PlanarBSplineData(int nCP, int degree) : CT_ShapeData()
{
    _nCP = nCP;
    _degree = degree;

    _controlPoints.resize(_nCP);
    _nodalSequence.resize(_nCP);
}

void CT_PlanarBSplineData::reset(int nCP, int degree)
{
    _nCP = nCP;
    _degree = degree;

    _controlPoints.resize(_nCP);
    _nodalSequence.resize(_nCP);
}

void CT_PlanarBSplineData::setCP(int index, double x, double y, double z)
{
    Q_ASSERT_X(index > 0 && index <= _nCP, "CT_PlanarBSplineData", "When getting control point, you must choose a valid index");

    _controlPoints[index](0) = x;
    _controlPoints[index](1) = y;
    _controlPoints[index](2) = z;
}

void CT_PlanarBSplineData::setCP(int index, const Eigen::Vector3d &value)
{
    setCP(index, value(0), value(1), value(2));
}

void CT_PlanarBSplineData::setNodalValue(int index, double value)
{
    Q_ASSERT_X(index > 0 && index <= _nCP, "CT_PlanarBSplineData", "When getting control point, you must choose a valid index");

    _nodalSequence[index] = value;
}

const Eigen::Vector3d &CT_PlanarBSplineData::getCPAt(int index) const
{
    Q_ASSERT_X(index > 0 && index <= _nCP, "CT_PlanarBSplineData", "When getting control point, you must choose a valid index");

    return _controlPoints.at(index);
}

double CT_PlanarBSplineData::getNodalValueAt(int index) const
{
    Q_ASSERT_X(index > 0 && index <= _nCP, "CT_PlanarBSplineData", "When getting control point, you must choose a valid index");

    return _nodalSequence.at(index);
}

CT_PlanarBSplineData *CT_PlanarBSplineData::clone() const
{
    CT_PlanarBSplineData* data = new CT_PlanarBSplineData(nCP(), degree());
    data->setCenter(getCenter());
    data->setDirection(getDirection());

    for (int i = 0 ; i < nCP() ; i++)
    {
        data->setCP(i, _controlPoints.at(i));
        data->setNodalValue(i, _nodalSequence.at(i));
    }

    return data;
}



