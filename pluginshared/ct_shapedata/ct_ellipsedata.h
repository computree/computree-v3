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

#ifndef CT_ELLIPSEDATA_H
#define CT_ELLIPSEDATA_H

#include "ct_shapedata.h"
#include "ct_linedata.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

class PLUGINSHAREDSHARED_EXPORT CT_EllipseData : public CT_ShapeData
{
public:
    CT_EllipseData();
    CT_EllipseData(const QVector3D &center, const CT_LineData &axisA, const CT_LineData &axisB);
    CT_EllipseData(const QVector3D &center, const CT_LineData &axisA, const CT_LineData &axisB, float error);

    const CT_LineData& getAxisA() const;
    const CT_LineData& getAxisB() const;
    float getError() const;

    CT_EllipseData* clone() const;

    /**
      * \brief Retourne les donnes d'une ellipse 2D  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  6.
      */
    static CT_EllipseData* staticCreateZAxisAlignedEllipseDataFromPointCloud(const CT_AbstractPointCloud *pointCloud,
                                                                             const CT_AbstractPointCloudIndex *pointCloudIndex);

private:

    CT_LineData _axisA;
    CT_LineData _axisB;
    float       _error;

    static void staticInitMatrix(double **m, int nlig, int ncol);
    static void staticDeleteMatrix(double **m, int nlig);
    static void staticGetCenter(double *pvec, float *cx, float *cy);
    static void staticGetAxisA(double *pvec, float cx, float cy, float *_x1, float *_y1, float *_x2, float *_y2);
    static void staticGetAxisB(double *pvec, float cx, float cy, float *_x1, float *_y1, float *_x2, float *_y2);
    static float staticIsZero(float x);
    static float staticGetQ(double *pvec);
    static void staticGetLine(float q, float xc, float yc, double *pvec, float *_x1, float *_y1, float *_x2, float *_y2);
    static void staticMultMatrix(double **m, double **g, double **mg);
    static void staticRotate(double **a, int i, int j, int k, int l, double tau, double s);
    static void staticJacobi(double **a, int n, double *d , double **v, int nrot);
    static void staticCholdc(double **a, int n, double **l);
    static int staticInverse(double **TB, double **InvB, int N);
    static void staticAperB(double **_A, double **_B, double **_res,
                       int _righA, int _colA, int _righB, int _colB);
    static void staticA_TperB(double **_A, double  **_B, double **_res,
                         int _righA, int _colA, int _righB, int _colB);
    static void staticAperB_T(double **_A, double **_B, double **_res,
                         int _righA, int _colA, int _righB, int _colB);

    static const float EL_INFINITY;
    static const float EL_ZERO;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        ar & _axisA & _axisB & _error;
    }
};

BOOST_CLASS_EXPORT_KEY(CT_EllipseData)
#else
};
#endif

#endif // CT_ELLIPSEDATA_H
