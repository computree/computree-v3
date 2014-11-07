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

#ifndef CT_CIRCLE2DDATA_H
#define CT_CIRCLE2DDATA_H

#include "ct_areashape2ddata.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

class PLUGINSHAREDSHARED_EXPORT CT_Circle2DDataPreProcessingAction
{
public:

    virtual void preProcessing(const CT_Point &point, CT_Point &newPoint) = 0;
};

class PLUGINSHAREDSHARED_EXPORT CT_Circle2DData : public CT_AreaShape2DData
{
public:
    CT_Circle2DData();
    CT_Circle2DData(const QVector2D &center, float radius);

    void setRadius(float radius);
    float getRadius() const;

    void getBoundingBox(QVector2D &min, QVector2D &max) const;

    bool contains(float x, float y) const;
    virtual double getArea() const;

    CT_Circle2DData* clone() const;

    CT_Circle2DData& operator= (const CT_Circle2DData& o);


private:

    float _radius;

};

#endif // CT_CIRCLE2DDATA_H
