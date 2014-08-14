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

#include "ct_pointcloudindexvector.h"

CT_PointCloudIndexVector::CT_PointCloudIndexVector(size_t size) : CT_AbstractPointCloudIndex(), CT_CloudIndexStdVectorT<CT_Point>(size)
{
}

CT_AbstractCloudIndex* CT_PointCloudIndexVector::copy() const
{
    size_t s = size();

    CT_PointCloudIndexVector *index = new CT_PointCloudIndexVector(s);

    std::vector<size_type> *vector = internalData();
    std::vector<size_type> *cpyVector = index->internalData();

    for(size_t i=0; i<s; ++i)
        (*cpyVector)[i] = (*vector)[i];

    index->setSortType(sortType());

    return index;
}
