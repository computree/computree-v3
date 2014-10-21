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

#include "ct_scene.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

CT_Scene::CT_Scene() : CT_AbstractItemDrawableWithPointCloud()
{
}

CT_Scene::CT_Scene(const CT_OutAbstractSingularItemModel *model,
                   const CT_AbstractResult *result) : CT_AbstractItemDrawableWithPointCloud(model, result)
{
}

CT_Scene::CT_Scene(const CT_OutAbstractSingularItemModel *model,
                   const CT_AbstractResult *result,
                   CT_AbstractCIR pcir) : CT_AbstractItemDrawableWithPointCloud(model, result)
{
    setPointCloudIndexRegistered(pcir);
}

CT_Scene::CT_Scene(const QString &modelName,
                   CT_AbstractResult *result) : CT_AbstractItemDrawableWithPointCloud(modelName, result)
{
}

CT_Scene::CT_Scene(const QString &modelName,
                   CT_AbstractResult *result,
                   CT_AbstractCIR pcir) : CT_AbstractItemDrawableWithPointCloud(modelName, result)
{
    setPointCloudIndexRegistered(pcir);
}

CT_Scene::~CT_Scene()
{
}

QString CT_Scene::getType() const
{
    return staticGetType();
}

QString CT_Scene::staticGetType()
{
    return CT_AbstractItemDrawableWithPointCloud::staticGetType() + "/CT_Scene";
}

int CT_Scene::getFastestIncrement() const
{
    return ((getPointCloudIndex() == NULL) || (getPointCloudIndex()->size() < 5000000)) ? 16 : 64;
}

CT_AbstractItemDrawable* CT_Scene::copy(const CT_OutAbstractItemModel *model,
                                        const CT_AbstractResult *result,
                                        CT_ResultCopyModeList copyModeList)
{
    CT_ResultCopyModeList::CopyMode copyMode = CT_ResultCopyModeList::CopyItemDrawableReference;

    if(!copyModeList.isEmpty())
    {
        copyMode = copyModeList.takeFirst();
    }

    if(copyMode == CT_ResultCopyModeList::DontCopyItemDrawable)
    {
        copyMode = CT_ResultCopyModeList::CopyItemDrawableReference;
    }

    CT_Scene *scene;

    if(copyMode == CT_ResultCopyModeList::CopyItemDrawableReference)
    {
        scene = new CT_Scene((const CT_OutAbstractSingularItemModel *)model,
                             result,
                             getPointCloudIndexRegistered());
        scene->setId(id());
    }
    else if(copyMode == CT_ResultCopyModeList::CopyItemDrawableCompletely)
    {
        scene = new CT_Scene((const CT_OutAbstractSingularItemModel *)model,
                             result);

        scene->setPointCloudIndexRegistered(PS_REPOSITORY->copyPointCloud(getPointCloudIndexRegistered(), CT_Repository::MemoryOptimized));
        scene->setId(id());
    }

    scene->setAlternativeDrawManager(getAlternativeDrawManager());

    return scene;
}

void CT_Scene::setPointCloudIndexRegistered(CT_AbstractCIR pcir)
{
    setPointCloudIndexRegisteredProtected(pcir);

    if(!pcir.isNull()) {updateBoundingBox();}
}

void CT_Scene::setTransformMatrix(const QMatrix4x4 &matrix)
{
    setMatrix4x4(matrix);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Scene)
#endif
