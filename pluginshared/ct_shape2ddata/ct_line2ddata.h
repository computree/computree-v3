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

#ifndef CT_LINE2DDATA_H
#define CT_LINE2DDATA_H

#include "ct_shape2ddata.h"

class PLUGINSHAREDSHARED_EXPORT CT_Line2DData : public CT_Shape2DData
{
public:
    CT_Line2DData();
    CT_Line2DData(const QVector2D &p1, const QVector2D &p2);

    const QVector2D &getP1() const;
    const QVector2D &getP2() const;

    double x1() const;
    double y1() const;

    double x2() const;
    double y2() const;

    double length() const;

    void getBoundingBox(QVector2D &min, QVector2D &max) const;

    CT_Line2DData* clone() const;

private:

    QVector2D   _p1;
    QVector2D   _p2;

};

#endif // CT_LINE2DDATA_H
