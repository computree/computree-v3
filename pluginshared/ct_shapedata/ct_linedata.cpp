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

#include "ct_linedata.h"

#include "ct_mathpoint.h"
#include <math.h>

#define ONE_DIV_THREE           0.33333333333333333333
#define ONE_DIV_ONE_POINT_FIVE  0.66666666666666666666
#define M_PI_MULT_2             6.28318530717958647692

CT_LineData::CT_LineData() : CT_ShapeData()
{
    _error = 0;
    _n_points = 0;
}

CT_LineData::CT_LineData(const QVector3D &p1, const QVector3D &p2) : CT_ShapeData(QVector3D(p1.x()+p2.x()/2.0,
                                                                                            p1.y()+p2.y()/2.0,
                                                                                            p1.z()+p2.z()/2.0),
                                                                                  QVector3D(p2.x()-p1.x(),
                                                                                            p2.y()-p1.y(),
                                                                                            p2.z()-p1.z()).normalized())
{
    _p1 = p1;
    _p2 = p2;
    _error = 0;
    _n_points = 0;
}

CT_LineData::CT_LineData(const QVector3D &p1, const QVector3D &p2, float error, int n) : CT_ShapeData(QVector3D(p1.x()+p2.x()/2.0,
                                                                                                         p1.y()+p2.y()/2.0,
                                                                                                         p1.z()+p2.z()/2.0),
                                                                                               QVector3D(p2.x()-p1.x(),
                                                                                                         p2.y()-p1.y(),
                                                                                                         p2.z()-p1.z()).normalized())
{
    _p1 = p1;
    _p2 = p2;
    _error = error;
    _n_points = n;
}

const QVector3D& CT_LineData::getP1() const
{
    return _p1;
}

const QVector3D& CT_LineData::getP2() const
{
    return _p2;
}

float CT_LineData::getError() const
{
    return _error;
}

double CT_LineData::x1() const
{
    return _p1.x();
}

double CT_LineData::y1() const
{
    return _p1.y();
}

double CT_LineData::z1() const
{
    return _p1.z();
}

double CT_LineData::x2() const
{
    return _p2.x();
}

double CT_LineData::y2() const
{
    return _p2.y();
}

double CT_LineData::z2() const
{
    return _p2.z();
}

double CT_LineData::length() const
{
    return CT_MathPoint::distance3D(_p1, _p2);
}

bool CT_LineData::intersectionWithRect3D  (float plan_x, float plan_y, float plan_z, QVector3D &vect_plan, float *xi, float *yi, float *zi)
{
    // intersection segment plan : http://www.softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm#intersect3D_SegPlane()
    QVector3D u(_p2.x() - _p1.x(), _p2.y() - _p1.y(), _p2.z() - _p1.z());
    QVector3D w(_p1.x() - plan_x, _p1.y() - plan_y, _p1.z() - plan_z);

    float D = QVector3D::dotProduct(vect_plan, u);
    float N = -QVector3D::dotProduct(vect_plan, w);

    if(fabs(D) > 0.00000001)
    {
        float sI = N / D;

        *xi = _p1.x() + sI * u.x();
        *yi = _p1.y() + sI * u.y();
        *zi = _p1.z() + sI * u.z();

        return true;
    }

    return false;
}

CT_LineData* CT_LineData::clone() const
{
    return new CT_LineData(getP1(), getP2(), getError(), n());
}

CT_LineData* CT_LineData::staticCreateLineDataFromPointCloud(const CT_AbstractPointCloud &pointCloud,
                                                             const CT_AbstractPointCloudIndex &pointCloudIndex)
{    
    Q_UNUSED(pointCloud)

    int indexSize = pointCloudIndex.size();

    if(indexSize < 2) {return NULL;}

    QList<CT_Point*> liste;

    for (int i = 0 ; i < indexSize ; i++)
        liste.append(&pointCloudIndex.tAt(i));

    CT_LineData* result = CT_LineData::staticCreateLineDataFromPointCloud<CT_Point>(liste);
    return result;

}
