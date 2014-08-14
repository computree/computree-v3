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

#ifndef CT_ABSTRACTPOINTCLOUD_H
#define CT_ABSTRACTPOINTCLOUD_H

#include "pluginShared_global.h"
#include "interfaces.h"

#include "ct_point.h"
#include "ct_cloud/abstract/ct_abstractcloudt.h"

/**
 * @brief A cloud of points
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointCloud : virtual public CT_AbstractCloudT<CT_Point>
{
public:
    CT_AbstractPointCloud();
    virtual ~CT_AbstractPointCloud() {}
};

#endif // CT_ABSTRACTPOINTCLOUD_H
