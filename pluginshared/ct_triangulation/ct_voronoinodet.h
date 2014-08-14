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

#ifndef CT_VORONOINODET_H
#define CT_VORONOINODET_H

#include "pluginShared_global.h"

#include <QSharedPointer>

class PLUGINSHAREDSHARED_EXPORT CT_VoronoiNodeT
{
public:

    static QSharedPointer<CT_VoronoiNodeT> create(float x, float y, float z)
    {
        QSharedPointer<CT_VoronoiNodeT> ptr(new CT_VoronoiNodeT(x, y, z));

        return ptr;
    }

    // retourne les coordonnees du point
    float getX() const;
    float getY() const;
    float getZ() const;

private:

    CT_VoronoiNodeT(float x, float y, float z);

    float _x, _y, _z;
};

#endif // CT_VORONOINODET_H
