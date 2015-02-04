/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#include "ct_polygon2ddata.h"

#include "ct_triangulation/ct_nodet.h"
#include "ct_triangulation/ct_trianglet.h"
#include "ct_triangulation/ct_edget.h"

#include <math.h>
#include <limits>
#include <algorithm>

CT_Polygon2DData::CT_Polygon2DData() : CT_AreaShape2DData()
{
}

CT_Polygon2DData::CT_Polygon2DData(const QVector<Eigen::Vector2d *> &vertices, bool copy) : CT_AreaShape2DData()
{

    int size = vertices.size();
    _vertices.resize(size);
    for (int i = 0 ; i < size ; i++)
    {
        Eigen::Vector2d* source = vertices.at(i);
        if (copy)
        {
            _vertices[i] = new Eigen::Vector2d(*source);
        } else {
            _vertices[i] = vertices.at(i);
        }
    }

    // Compute Centroid and affect to center
    computeCentroid();
}

void CT_Polygon2DData::computeCentroid()
{
    double signedArea = 0.0;
    double a = 0.0;

    Eigen::Vector2d ptC;
    ptC(0) = 0;
    ptC(1) = 0;

    _minX = std::numeric_limits<double>::max();
    _minY = std::numeric_limits<double>::max();
    _maxX = -std::numeric_limits<double>::max();
    _maxY = -std::numeric_limits<double>::max();

    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = _vertices.size();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);

        if ((*pt2)(0) < _minX) {_minX = (*pt2)(0);}
        if ((*pt2)(0) > _maxX) {_maxX = (*pt2)(0);}
        if ((*pt2)(1) < _minY) {_minY = (*pt2)(1);}
        if ((*pt2)(1) > _maxY) {_maxY = (*pt2)(1);}

        a = (*pt1)(0)*(*pt2)(1) - (*pt2)(0)*(*pt1)(1);
        signedArea += a;

        ptC += (*pt1 + *pt2)*a    ;
        pt1 = pt2;
    }

    signedArea *= 0.5;
    ptC /= (6.0*signedArea);

    setCenter(ptC);
}

CT_Polygon2DData::~CT_Polygon2DData()
{
    qDeleteAll(_vertices);
}


CT_Polygon2DData* CT_Polygon2DData::clone() const
{
    return new CT_Polygon2DData(_vertices);
}

void CT_Polygon2DData::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min(0) = _minX;
    min(1) = _minY;
    min(2) = 0;
    max(0) = _maxX;
    max(1) = _maxY;
    max(2) = 0;
}

void CT_Polygon2DData::expand(double buffer)
{
    int size = getVerticesNumber();
    if (size < 3) {return;}

    Eigen::Vector2d pt1, pt2, ptTrans, ptTang, direction;

    for (int i = 0 ; i < size ; i++)
    {
        Eigen::Vector2d* pt = _vertices.at(i);

        if (i > 0) {pt1 = *(_vertices.at(i - 1));}
        else {pt1 = *(_vertices.last());}

        if (i < size - 1) {pt2 = *(_vertices.at(i + 1));}
        else {pt2 = *(_vertices.first());}

        ptTang = pt2 - pt1;
        ptTang.normalize();

        direction(0) = -ptTang(1);
        direction(1) = ptTang(0);

        ptTrans = *pt + direction*buffer;

        double distBefore = (*pt - _center).norm();
        double distAfter  = (ptTrans - _center).norm();

        if (((buffer > 0) && (distAfter < distBefore)) || ((buffer < 0) && (distAfter > distBefore))) {ptTrans = *pt - direction*buffer;}

        *pt = ptTrans;
    }

    computeCentroid();
}

double CT_Polygon2DData::getAreaIfNotSelfIntersecting() const
{
    return fabs(getSignedArea());
}

double CT_Polygon2DData::getSignedArea() const
{
    double area = 0;
    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);
        area += (*pt1)(0)*(*pt2)(1) - (*pt2)(0)*(*pt1)(1);
        pt1 = pt2;
    }

    return 0.5*area;
}

double CT_Polygon2DData::getPerimeter() const
{
    double perimeter = 0;
    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);
        perimeter += sqrt(pow((*pt1)(0) - (*pt2)(0), 2) + pow((*pt1)(1) - (*pt2)(1), 2));
        pt1 = pt2;
    }

    return perimeter;
}

double CT_Polygon2DData::getArea() const
{
    return getAreaIfNotSelfIntersecting();
}

bool CT_Polygon2DData::contains(double x, double y) const
{
    if (x < _minX) {return false;}
    if (x > _maxX) {return false;}
    if (y < _minY) {return false;}
    if (y > _maxY) {return false;}

    Eigen::Vector2d *pt1 = NULL;
    Eigen::Vector2d *pt2 = NULL;
    bool result = false;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);


        if (((((*pt2)(1) <= y) && (y < (*pt1)(1))) || (((*pt1)(1) <= y) && (y < (*pt2)(1)))) &&
                (x < (((*pt1)(0) - (*pt2)(0))*(y - (*pt2)(1))/((*pt1)(1) - (*pt2)(1)) + (*pt2)(0))))
        {
            result = !result;
        }

        pt1 = pt2;
    }
    return result;
}

void CT_Polygon2DData::draw(PainterInterface &painter, bool drawPoints, bool drawLines, double zPlane) const
{
    if(drawPoints || drawLines)
    {
        const QVector<Eigen::Vector2d*>&vertices = getVertices();
        int size = vertices.size();

        Eigen::Vector2d *pt1 = vertices.last();
        for (int i = 0 ; i < size ; i++)
        {
            Eigen::Vector2d *pt2 = vertices.at(i);
            if(drawPoints)
            {
                painter.drawPoint((*pt2)(0), (*pt2)(1), zPlane);
            }
            if(drawLines)
            {
                painter.drawLine((*pt1)(0), (*pt1)(1), zPlane, (*pt2)(0), (*pt2)(1), zPlane);
            }
            pt1 = pt2;
        }
    }
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_PointCloudIndexVector *indices)
{
    // Compute triangulation
    CT_DelaunayT delaunay;

    size_t size = indices->size();

    for (size_t i = 0 ; i < size ; i++)
    {
        CT_Point &point = indices->tAt(i);
        float* pt = &point(0);

        delaunay.insertNode(CT_NodeT::create(pt, NULL, false));
    }
    return createConvexHull(delaunay);
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_DelaunayT &triangulation)
{
    QVector<Eigen::Vector2d*> convexHull;

    // Retrieve Convex Hull
    QList< QSharedPointer<CT_NodeT> > nodes = triangulation.getHullNodes();

    int size = nodes.size();

    if (size < 3) {return NULL;}

    for (int i = 0 ; i < size ; i++)
    {
        QSharedPointer<CT_NodeT> node = nodes.at(i);
        float* point = node.data()->getPoint();

        Eigen::Vector2d *vertice = new Eigen::Vector2d(point[0], point[1]);
        convexHull.append(vertice);
    }

    return new CT_Polygon2DData(convexHull);
}



void CT_Polygon2DData::orderPointsByXY(QList<Eigen::Vector2d*> &pointList)
{
    std::sort(pointList.begin(), pointList.end(), compare);
}

bool CT_Polygon2DData::compare(const Eigen::Vector2d* p1, const Eigen::Vector2d* p2)
{
     return ((*p1)(0) < (*p2)(0) || ((*p1)(0) == (*p2)(0) && (*p1)(1) < (*p2)(1)));
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
CT_Polygon2DData* CT_Polygon2DData::createConvexHull(QList<Eigen::Vector2d*> &orderedCandidates)
{
    // Adapted from http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain

    int n = orderedCandidates.size();

    if (n < 3) {return NULL;}

    // Build lower hull
    QVector<Eigen::Vector2d*> Hlower(n);
    int l = 0;
    for (int i = 0 ; i < n ; ++i)
    {
        while (l >= 2 && cross(Hlower[l-2], Hlower[l-1], orderedCandidates[i]) <= 0) {l--;}
        Hlower[l++] = orderedCandidates[i];
    }
    Hlower.resize(l - 1);

    // Build upper hull
    QVector<Eigen::Vector2d*> Hupper(n);
    int u = 0;
    for (int i = n-1; i >= 0; i--)
    {
        while (u >= 2 && cross(Hupper[u-2], Hupper[u-1], orderedCandidates[i]) <= 0) {u--;}
        Hupper[u++] = orderedCandidates[i];
    }
    Hupper.resize(u - 1);

    Hlower += Hupper;

    if (Hlower.size() < 3) {return NULL;}

    return  new CT_Polygon2DData(Hlower);
}

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
double CT_Polygon2DData::cross(const Eigen::Vector2d* O, const Eigen::Vector2d* A, const Eigen::Vector2d* B)
{
    return ((*A)(0) - (*O)(0)) * ((*B)(1) - (*O)(1)) - ((*A)(1) - (*O)(1)) * ((*B)(0) - (*O)(0));
}
