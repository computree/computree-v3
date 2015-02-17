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

#include "ct_abstractsteploadfileinscene.h"

#include "ct_stepinitializedata.h"

#include "ct_outstdgroupmodel.h"
#include "ct_outstdsingularitemmodel.h"
#include "ct_outresultmodelgroup.h"

#include "ct_scene.h"
#include "ct_point.h"
#include "ct_standarditemgroup.h"

#include "ct_resultgroup.h"

#include "ct_global/ct_context.h"
#include "ct_coordinates/ct_defaultcoordinatesystem.h"

#include "ct_iterator/ct_mutablepointiterator.h"

#include "qdebug.h"

#include <QFile>
#include <QDir>
#include <math.h>
#include <limits>

#define DEF_SearchResult    "r"
#define DEF_SearchGroup     "SG"
#define DEF_SearchScene     "Scene"

CT_AbstractStepLoadFileInScene::CT_AbstractStepLoadFileInScene(CT_StepInitializeData &dataInit) : CT_AbstractStepLoadFile(dataInit)
{
}

void CT_AbstractStepLoadFileInScene::createOutResultModelListProtected()
{
    CT_OutStdGroupModel *root = new CT_OutStdGroupModel(DEF_SearchGroup);
    CT_OutStdSingularItemModel *scene = new CT_OutStdSingularItemModel(DEF_SearchScene, new CT_Scene());

    root->addItem(scene);

    QList<CT_OutAbstractSingularItemModel *> list = createOtherOutResultListProtected(DEF_SearchScene);

    while(!list.isEmpty())
        root->addItem(list.takeFirst());

    addOutResultModel(new CT_OutResultModelGroup(DEF_SearchResult, root, "CT_ResultPointCloud"));
}

void CT_AbstractStepLoadFileInScene::compute()
{
    setErrorCode(-1);

    if(QFile::exists(getFilePath()))
    {
        QFile f(getFilePath());

        if(f.open(QIODevice::ReadOnly))
        {
            setErrorCode(0);

            int offset = readHeaderFile(f);

            if(offset != -1)
                readDataFile(f, offset);

            f.close();
        }
    }
}

void CT_AbstractStepLoadFileInScene::readDataFile(QFile &f, int offset, bool little_endian)
{
    qint64 filesize = f.size();
    quint64 a = 0;
    //quint64 seek_to = offset;
    bool seek_ok = true;

    // un tableau de n_points
    quint64 n_points = (filesize - offset) / 26;

    /*if(_number_of_points_to_ignore > 0)
    {
        n_points = n_points/((quint64)_number_of_points_to_ignore);

        if(n_points == 0)
        {
            n_points = 1;
        }
    }*/

    CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(n_points);

    CT_MutablePointIterator it(pcir);
    CT_Point pReaded;
    GLuint indexOfCoordinateSystem = 0;

    double xmin = std::numeric_limits<float>::max();
    double ymin = std::numeric_limits<float>::max();
    double zmin = std::numeric_limits<float>::max();

    double xmax = -xmin;
    double ymax = -xmin;
    double zmax = -xmin;

    char d_data[8];

    while((a < n_points)
            && (seek_ok)
            && (!isStopped()))
    {
        f.read(d_data, 8);
        double x = getDouble(d_data, little_endian);
        f.read(d_data, 8);
        double y = getDouble(d_data, little_endian);
        f.read(d_data, 8);
        double z = getDouble(d_data, little_endian);
        f.read(d_data, 2);
        //getShort(d_data); // reflectance

        if(a == 0) {
            if (fabs(x) > 1000 || fabs(y) > 1000 || fabs(z) > 1000)
                indexOfCoordinateSystem = (new CT_DefaultCoordinateSystem(x, y, z, this))->indexInManager();
        }

        pReaded(0) = x;
        pReaded(1) = y;
        pReaded(2) = z;

        it.next();
        it.replaceCurrentPoint(pReaded, indexOfCoordinateSystem);

        if (x<xmin) {xmin = (float)x;}

        if (x>xmax) {xmax = (float)x;}

        if (y<ymin) {ymin = (float)y;}

        if (y>ymax) {ymax = (float)y;}

        if (z<zmin) {zmin = (float)z;}

        if (z>zmax) {zmax = (float)z;}

        // progres de 0 a 100
        setProgress((a*100)/n_points);

        /*if(_number_of_points_to_ignore > 0)
        {
            seek_to += 26*_number_of_points_to_ignore;

            seek_ok = f.seek(seek_to);
        }*/

        ++a;
    }

    createOutResult(pcir, xmin, xmax, ymin, ymax, zmin, zmax);

}

void CT_AbstractStepLoadFileInScene::createOutResult(CT_PCIR pcir, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, QList<CT_AbstractSingularItemDrawable*> list)
{
    CT_ResultGroup *out_res = getOutResultList().first();

    CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_SearchGroup, out_res);

    CT_Scene *scene = new CT_Scene(DEF_SearchScene, out_res, pcir);
    scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

    group->addItemDrawable(scene);

    while(!list.isEmpty())
        group->addItemDrawable(list.takeFirst());

    out_res->addGroup(group);
}
