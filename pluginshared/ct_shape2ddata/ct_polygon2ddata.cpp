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

#include <math.h>
#include <limits>
#include <algorithm>

CT_Polygon2DData::CT_Polygon2DData() : CT_AreaShape2DData()
{
}

CT_Polygon2DData::CT_Polygon2DData(const QVector<QVector2D*> &vertices) : CT_AreaShape2DData()
{

    int size = vertices.size();
    _vertices.resize(size);
    for (int i = 0 ; i < size ; i++)
    {
        QVector2D* source = vertices.at(i);
        _vertices[i] = new QVector2D(source->x(), source->y());
    }

    // Compute Centroid and affect to center
    computeCentroid();
}

void CT_Polygon2DData::computeCentroid()
{
    double signedArea = 0.0;
    double a = 0.0;
    double cx = 0;
    double cy = 0;

    _minX = std::numeric_limits<float>::max();
    _minY = std::numeric_limits<float>::max();
    _maxX = -std::numeric_limits<float>::max();
    _maxY = -std::numeric_limits<float>::max();

    QVector2D *pt1 = NULL;
    QVector2D *pt2 = NULL;

    int size = _vertices.size();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);

        if (pt2->x() < _minX) {_minX = pt2->x();}
        if (pt2->x() > _maxX) {_maxX = pt2->x();}
        if (pt2->y() < _minY) {_minY = pt2->y();}
        if (pt2->y() > _maxY) {_maxY = pt2->y();}

        a = pt1->x()*pt2->y() - pt2->x()*pt1->y();
        signedArea += a;
        cx += (pt1->x() + pt2->x())*a;
        cy += (pt1->y() + pt2->y())*a;

        pt1 = pt2;
    }

    signedArea *= 0.5;
    cx /= (6.0*signedArea);
    cy /= (6.0*signedArea);

    setCenter(QVector2D(cx, cy));
}

CT_Polygon2DData::~CT_Polygon2DData()
{
    qDeleteAll(_vertices);
}


CT_Polygon2DData* CT_Polygon2DData::clone() const
{
    return new CT_Polygon2DData(_vertices);
}

void CT_Polygon2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(_minX);
    min.setY(_minY);
    max.setX(_maxX);
    max.setY(_maxY);
}

void CT_Polygon2DData::expand(float buffer)
{
    int size = getVerticesNumber();
    for (int i = 0 ; i < size ; i++)
    {
        QVector2D* pt = _vertices.at(i);

        if (pt->x() > getCenter().x()) {pt->setX(pt->x() + buffer);}
        if (pt->x() < getCenter().x()) {pt->setX(pt->x() - buffer);}
        if (pt->y() > getCenter().y()) {pt->setY(pt->y() + buffer);}
        if (pt->y() < getCenter().y()) {pt->setY(pt->y() - buffer);}
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
    QVector2D *pt1 = NULL;
    QVector2D *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);
        area += pt1->x()*pt2->y() - pt2->x()*pt1->y();
        pt1 = pt2;
    }

    return 0.5*area;
}

double CT_Polygon2DData::getPerimeter() const
{
    double perimeter = 0;
    QVector2D *pt1 = NULL;
    QVector2D *pt2 = NULL;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);
        perimeter += sqrt(pow(pt1->x() - pt2->x(), 2) + pow(pt1->y() - pt2->y(), 2));
        pt1 = pt2;
    }

    return perimeter;
}

double CT_Polygon2DData::getArea() const
{
    return getAreaIfNotSelfIntersecting();
}

bool CT_Polygon2DData::contains(float x, float y) const
{
    if (x < _minX) {return false;}
    if (x > _maxX) {return false;}
    if (y < _minY) {return false;}
    if (y > _maxY) {return false;}

    QVector2D *pt1 = NULL;
    QVector2D *pt2 = NULL;
    bool result = false;

    int size = getVerticesNumber();
    pt1 = _vertices.at(size - 1);
    for (int i = 0 ; i < size ; i++)
    {
        pt2 = _vertices.at(i);


        if ((((pt2->y() <= y) && (y < pt1->y())) || ((pt1->y() <= y) && (y < pt2->y()))) &&
                (x < ((pt1->x() - pt2->x())*(y - pt2->y())/(pt1->y() - pt2->y()) + pt2->x())))
        {
            result = !result;
        }

        pt1 = pt2;
    }
    return result;
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_PointCloudIndexVector *indices)
{
    // Compute triangulation
    CT_DelaunayT delaunay;

    size_t size = indices->size();

    for (size_t i = 0 ; i < size ; i++)
    {
        CT_Point &point = indices->tAt(i);
        float* pt = &point.data[0];

        delaunay.insertNode(CT_NodeT::create(pt, NULL, false));
    }
    return createConvexHull(delaunay);
}

CT_Polygon2DData* CT_Polygon2DData::createConvexHull(const CT_DelaunayT &triangulation)
{
    QVector<QVector2D*> convexHull;

    // Retrieve Convex Hull
    QList< QSharedPointer<CT_NodeT> > nodes = triangulation.getHullNodes();

    int size = nodes.size();

    if (size < 3) {return NULL;}

    for (int i = 0 ; i < size ; i++)
    {
        QSharedPointer<CT_NodeT> node = nodes.at(i);
        float* point = node.data()->getPoint();

        QVector2D *vertice = new QVector2D(point[0], point[1]);
        convexHull.append(vertice);
    }

    return new CT_Polygon2DData(convexHull);
}



void CT_Polygon2DData::orderPointsByXY(QList<QVector2D*> &pointList)
{
    std::sort(pointList.begin(), pointList.end(), compare);
}

bool CT_Polygon2DData::compare(const QVector2D* p1, const QVector2D* p2)
{
     return (p1->x() < p2->x() || (p1->x() == p2->x() && p1->y() < p2->y()));
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
CT_Polygon2DData* CT_Polygon2DData::createConvexHull(QList<QVector2D*> &orderedCandidates)
{
    // Adapted from http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain

    int n = orderedCandidates.size();

    if (n < 3) {return NULL;}

    // Build lower hull
    QVector<QVector2D*> Hlower(n);
    int l = 0;
    for (int i = 0 ; i < n ; ++i)
    {
        while (l >= 2 && cross(Hlower[l-2], Hlower[l-1], orderedCandidates[i]) <= 0) {l--;}
        Hlower[l++] = orderedCandidates[i];
    }
    Hlower.resize(l - 1);

    // Build upper hull
    QVector<QVector2D*> Hupper(n);
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
float CT_Polygon2DData::cross(const QVector2D* O, const QVector2D* A, const QVector2D* B)
{
    return (A->x() - O->x()) * (B->y() - O->y()) - (A->y() - O->y()) * (B->x() - O->x());
}
