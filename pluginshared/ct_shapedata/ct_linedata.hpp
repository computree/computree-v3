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

#ifndef CT_LINEDATA_HPP
#define CT_LINEDATA_HPP

#include "ct_shapedata/ct_linedata.h"
#include "qdebug.h"
#include "ct_math/ct_mathpoint.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#define M_PI_MULT_2             6.28318530717958647692
#define ONE_DIV_THREE           0.33333333333333333333
#define ONE_DIV_ONE_POINT_FIVE  0.66666666666666666666

template<typename XYZpoint>
CT_LineData* CT_LineData::staticCreateLineDataFromPointCloud(QList<XYZpoint*> &l_gp)
{
    double x;
    double y;
    double z;

    quint64 n = 0;

    double Xm = 0;
    double Xm_carre;

    double Ym = 0;
    double Ym_carre;

    double Zm = 0;
    double Zm_carre;

    double Xm_m_Ym;
    double Xm_m_Zm;
    double Ym_m_Zm;

    double Sxx = 0;
    double Syy = 0;
    double Szz = 0;
    double Sxy = 0;
    double Sxz = 0;
    double Syz = 0;

    double Sxy_carre;
    double Sxz_carre;
    double Syz_carre;

    double c0;
    double c1;
    double c2;
    double c3;

    double r;
    double s;
    double t;
    double p;
    double q;
    double R;

    int n_a;
    double *a_tab;

    double a;
    double a_carre;
    double b;
    double b_carre;
    double u;
    double v;
    double w;

    double temp;
    double temp1;
    double temp2;
    double temp3;
    double temp4;
    double temp5;

    QListIterator<XYZpoint*> it(l_gp);

    while(it.hasNext())
    {
        XYZpoint *point = it.next();

        if (point != NULL)
        {
            x = CT_MathPoint::pX<XYZpoint>(*point);
            y = CT_MathPoint::pY<XYZpoint>(*point);
            z = CT_MathPoint::pZ<XYZpoint>(*point);

            Xm += x;
            Ym += y;
            Zm += z;

            ++n;
        } else {
            qDebug() << "Problème : Pointer de point Null";
        }

    }

    temp1 = n;

    Xm /= temp1;
    Ym /= temp1;
    Zm /= temp1;

    Xm_carre = Xm*Xm;
    Ym_carre = Ym*Ym;
    Zm_carre = Zm*Zm;

    Xm_m_Ym = Xm*Ym;
    Xm_m_Zm = Xm*Zm;
    Ym_m_Zm = Ym*Zm;

    it.toFront();

    while(it.hasNext())
    {
        XYZpoint *point = it.next();

        if (point != NULL)
        {
            x = CT_MathPoint::pX<XYZpoint>(*point);
            y = CT_MathPoint::pY<XYZpoint>(*point);
            z = CT_MathPoint::pZ<XYZpoint>(*point);

            Sxx += (x*x) - (Xm_carre);
            Syy += (y*y) - (Ym_carre);
            Szz += (z*z) - (Zm_carre);

            Sxy += (x*y) - (Xm_m_Ym);
            Sxz += (x*z) - (Xm_m_Zm);
            Syz += (y*z) - (Ym_m_Zm);
        }

    }

    Sxx /= temp1;
    Syy /= temp1;
    Szz /= temp1;

    Sxy /= temp1;
    Sxz /= temp1;
    Syz /= temp1;

    Sxy_carre = Sxy*Sxy;
    Sxz_carre = Sxz*Sxz;
    Syz_carre = Syz*Syz;

    c0 = Sxy*Sxz * (Szz-Syy) + Syz * (Sxy_carre - Sxz_carre);
    c1 = Sxz*Syz * ((2.0*Sxx)-Syy-Szz) + Sxy * (-Sxy_carre-Sxz_carre+2*Syz_carre) + Sxy * (Szz-Sxx)*(Szz-Syy);
    c2 = Sxy*Sxz * (Sxx+Syy-(2.0*Szz)) + Syz * (Sxz_carre+Syz_carre-(2*Sxy_carre)) + Syz * (Sxx-Syy)*(Szz-Sxx);
    c3 = Sxz*Syz * (Syy-Sxx) + Sxy*(Sxz_carre-Syz_carre);

    r = c2/c3;
    s = c1/c3;
    t = c0/c3;
    p = s - ((r*r) / 3.0);
    q = ((2.0 * (r*r*r)) / 27.0) - ((r*s) / 3.0) + t;
    R = ((q*q) / 4.0) + ((p*p*p) / 27.0);

    if(R > 0)
    {
        n_a = 1;
        a_tab = new double[n_a];
        a_tab[0] = (-r/3.0) + pow(sqrt((-q/2.0) + pow(sqrt(R), 0.5)), ONE_DIV_THREE) + pow(sqrt((-q/2.0) - pow(sqrt(R), 0.5)), ONE_DIV_THREE);
    }
    else
    {
        double ro = sqrt(-(p*p*p)/27.0);
        double phi = acos(-q/(2.0*ro));

        temp1 = (-r/3.0);
        temp2 = 2 * pow(sqrt(ro), ONE_DIV_ONE_POINT_FIVE); // dans le pdf c'est 1.0/3.0 (erreur dans le pdf apparemment)

        n_a = 3;
        a_tab = new double[n_a];

        a_tab[0] = temp1 + (temp2 * cos(phi/3.0));
        a_tab[1] = temp1 + (temp2 * cos((phi + M_PI_MULT_2)/3.0));
        a_tab[2] = temp1 + (temp2 * cos((phi + 4*M_PI)/3.0));
    }

    temp5 = 999999999;

    for(int i=0; i<n_a; ++i)
    {
        a = a_tab[i];
        a_carre = a*a;

        b = ((a * (Szz-Sxx)) + (1 - a_carre) * Sxz) / (Sxy + (a * Syz));
        b_carre = b*b;

        temp = 1.0 / (1.0 + a_carre + b_carre);

        double ut = temp * (((1.0 + b_carre)*Xm) - (a*b*Ym) + (a*Zm));
        double vt = temp * ((-a*b*Xm) + ((1.0+a_carre)*Ym) + (b*Zm));
        double wt = temp * ((a*Xm) + (b*Ym) + ((a_carre + b_carre)*Zm));

        temp4 = 0;

        it.toFront();

        while(it.hasNext())
        {
            XYZpoint *point = it.next();

            if (point != NULL)
            {
                x = CT_MathPoint::pX<XYZpoint>(*point);
                y = CT_MathPoint::pY<XYZpoint>(*point);
                z = CT_MathPoint::pZ<XYZpoint>(*point);

                temp1 = (temp * (((1.0 + b_carre)*x) - (a*b*y) + (a*z))) - ut;
                temp2 = (temp * ((-a*b*x) + ((1.0+a_carre)*y) + (b*z))) - vt;
                temp3 = (temp * ((a*x) + (b*y) + ((a_carre + b_carre)*z))) - wt;

                temp4 += (temp1*temp1) + (temp2*temp2) + (temp3*temp3);
            }
        }

        if(temp4 < temp5)
        {
            temp5 = temp4;
            u = ut;
            v = vt;
            w = wt;
        }
    }

    delete [] a_tab;

    QVector3D p1(Xm, Ym, Zm);
    QVector3D p2(u, v, w);

    return new CT_LineData(p1, p2, temp5, n);

}

#endif // CT_LINEDATA_HPP
