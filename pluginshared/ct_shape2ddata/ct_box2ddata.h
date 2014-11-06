/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#ifndef CT_BOX2DDATA_H
#define CT_BOX2DDATA_H

#include "ct_areashape2ddata.h"

class PLUGINSHAREDSHARED_EXPORT CT_Box2DData : public CT_AreaShape2DData
{
public:
    CT_Box2DData();
    CT_Box2DData(const QVector2D &center, float height, float width);

    float getHeight() const;
    float getWidth() const;

    void getBoundingBox(QVector2D &min, QVector2D &max) const;

    bool contains(float x, float y) const;
    virtual double getArea() const;

    CT_Box2DData* clone() const;

    CT_Box2DData& operator= (const CT_Box2DData& o);

private:

    float       _width;  // selon l'axe de x
    float       _height; // selon l'axe de y
};

#endif // CT_BOX2DDATA_H
