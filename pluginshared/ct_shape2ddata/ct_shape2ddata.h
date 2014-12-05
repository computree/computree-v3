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

#ifndef CT_SHAPE2DDATA_H
#define CT_SHAPE2DDATA_H

#include "pluginShared_global.h"
#include <QVector2D>

/**
  * Reprsente les donnes d'une forme géomtrique 2D
  */
class PLUGINSHAREDSHARED_EXPORT CT_Shape2DData
{
public:

    CT_Shape2DData();
    CT_Shape2DData(const QVector2D &center);
    CT_Shape2DData(const float &x, const float &y);
    virtual ~CT_Shape2DData();

    const QVector2D& getCenter() const;
    void setCenter(const QVector2D &center);

    virtual void getBoundingBox(QVector2D &min, QVector2D &max) const = 0;

protected:

    QVector2D   _center;

    friend class CT_AbstractShape2D;

};

#endif // CT_SHAPE2DDATA_H
