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

#ifndef CT_POINTCLOUDSTDVECTOR_H
#define CT_POINTCLOUDSTDVECTOR_H

#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 * A cloud of points that use a std::vector
 */
class PLUGINSHAREDSHARED_EXPORT CT_PointCloudStdVector :  public CT_AbstractPointCloud, public CT_StandardCloudStdVectorT<CT_Point>
{
public:
    CT_PointCloudStdVector(const size_t &size = 0);

    CT_AbstractCloud* copy() const;
};

#endif // CT_POINTCLOUDSTDVECTOR_H
