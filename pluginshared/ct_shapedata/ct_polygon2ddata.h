/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#ifndef CT_POLYGON2DDATA_H
#define CT_POLYGON2DDATA_H

#include "ct_shapedata/ct_shapedata.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include "ct_triangulation/ct_delaunayt.h"


#include <QVector>
#include <QVector2D>
#include <QStack>

class PLUGINSHAREDSHARED_EXPORT CT_Polygon2DData : public CT_ShapeData
{
public:

    CT_Polygon2DData();

    CT_Polygon2DData(const QVector<QVector2D *> &vertices);

    ~CT_Polygon2DData();

    CT_Polygon2DData* clone() const;

    void computeCentroid();

    void expand(float buffer);

    inline const QVector<QVector2D*>& getVertices() const {return _vertices;}

    double getAreaIfNotSelfIntersecting();
    double getSignedArea();
    double getPerimeter();
    inline int getVerticesNumber() {return _vertices.size();}

    bool contains(float x, float y);

    static CT_Polygon2DData* createConvexHull(const CT_PointCloudIndexVector *indices);
    static CT_Polygon2DData* createConvexHull(const CT_DelaunayT &triangulation);

    static void orderPointsByXY(QList<QVector2D *> &pointList);
    // Use Andrew's monotone chain convex hull algorithm
    static CT_Polygon2DData* createConvexHull(QList<QVector2D *> &orderedCandidates);
    static CT_Polygon2DData* chainHull_2D(QList<QVector2D*> &orderedCandidates);

private:
    QVector<QVector2D*> _vertices;

    float               _minX;
    float               _maxX;
    float               _minY;
    float               _maxY;

    friend class CT_Polygon2D;

    static float cross(const QVector2D* O, const QVector2D* A, const QVector2D* B);
    static bool compare(const QVector2D* p1, const QVector2D* p2);

};

#endif // CT_POLYGON2DDATA_H
