/****************************************************************************

 Copyright (C) 2010-2015 the Office National des Forêts (ONF), France
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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

#include "ct_delaunayoutline.h"

CT_DelaunayOutline::CT_DelaunayOutline()
{
}

CT_DelaunayOutline::~CT_DelaunayOutline()
{
    _vertices.clear();
}

void CT_DelaunayOutline::addVertex(CT_DelaunayVertex *vt)
{
    for (int i = 0 ; i < _vertices.size () ; i++)
    {
        CT_DelaunayVertex* vt2 = _vertices.at(i);
        if (vt2->equals (vt)) {return;}
    }

    _vertices.append(vt);
}

double CT_DelaunayOutline::area()
{
    double sum = 0;
    CT_DelaunayVertex* v1;
    CT_DelaunayVertex* v2;

    _vertices.append(_vertices.at(0));

    for (int i = 0 ; i < _vertices.size () - 1 ; i++)
    {
        v1 = _vertices.at(i);
        v2 = _vertices.at(i+1);
        sum += (v1->x() * v2->y()) - (v1->y() * v2->x());
    }

    _vertices.removeLast();

    return std::abs(sum / 2.0);
}

bool CT_DelaunayOutline::contains(double x, double y)
{

    double a, b, a2, b2, xInter, yInter, maxx, maxy, minx, miny;
    bool ok = false;
    CT_DelaunayVertex* vt1;
    CT_DelaunayVertex* vt2;
    int nbInter = 0;
    int nbInterVertices = 0;

    // if the point (x,y) is a vertex, it's contained in the polygon
    for (int i = 0 ; i < _vertices.size() ; i++)
    {
        vt1 = _vertices.at(i);
        if ((x == vt1->x()) && (y == vt1->y())) {return true;}
    }

    _vertices.append(_vertices.at(0));

    while (!ok)
    {
        a = ((double) std::rand() / (RAND_MAX));
        b = y - a*x;

        ok = true;
        nbInter = 0;
        nbInterVertices = 0;

        for (int i = 0; i < (_vertices.size() - 1) ; i++)
        {
            vt1 = _vertices.at(i);
            vt2 = _vertices.at(i+1);

            if (vt1->x() != vt2->x())
            {
                a2 = (vt1->y() - vt2->y()) / (vt1->x() - vt2->x());
                b2 = vt1->y() - a2*vt1->x();

                if (a != a2)
                {
                    xInter = (b2-b)/(a-a2);
                } else {
                    ok = false;
                    break;
                }
            } else
            {
                xInter = vt1->x();
            }

            yInter = a*xInter + b;

            if (xInter <= x)
            {

                if (vt1->x() > vt2->x()) {maxx = vt1->x(); minx = vt2->x();}
                else               {maxx = vt2->x(); minx = vt1->x();}

                if (vt1->y() > vt2->y()) {maxy = vt1->y(); miny = vt2->y();}
                else               {maxy = vt2->y(); miny = vt1->y();}

                if ((xInter >= minx) && (xInter <= maxx) && (yInter >= miny) && (yInter <= maxy)) {

                    nbInter = nbInter + 1;
                }
            }


            for (int j = 0 ; j <_vertices.size() - 1 ; j++)
            {
                vt1 = _vertices.at(j);

                if ((vt1->x() == xInter) && (vt1->y() == yInter))
                {
                    ok = false;
                    break;
                }
            }
        }
    }

    _vertices.removeLast();

    if ((nbInter == 0) || ((nbInter % 2) == 0)) {return false;}
    else                                        {return true;}
}

CT_Polygon2DData *CT_DelaunayOutline::getShape()
{
    QVector<Eigen::Vector2d *> vertices;

    for (int i = 0 ; i < _vertices.size() ; i++)
    {
        CT_DelaunayVertex* vt = _vertices.at(i);
        vertices.append(new Eigen::Vector2d(vt->x(), vt->y()));
    }

    return new CT_Polygon2DData(vertices, false);
}

