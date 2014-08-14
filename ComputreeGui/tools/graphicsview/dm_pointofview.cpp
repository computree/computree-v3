/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "dm_pointofview.h"
#include <QStringList>

DM_PointOfView::DM_PointOfView()
{
    _quaternionInformation = false;
}

DM_PointOfView::DM_PointOfView(QString name, double x, double y, double z, double rotX, double rotY, double rotZ)
{
    _name = name;
    _x = x;
    _y = y;
    _z = z;
    _rotX = rotX;
    _rotY = rotY;
    _rotZ = rotZ;
    _quaternionInformation = false;
    _q0 = 0;
    _q1 = 0;
    _q2 = 0;
    _q3 = 0;
}

DM_PointOfView::DM_PointOfView(QString name, double x, double y, double z, double rotX, double rotY, double rotZ, double q0, double q1, double q2, double q3)
{
    _name = name;
    _x = x;
    _y = y;
    _z = z;
    _rotX = rotX;
    _rotY = rotY;
    _rotZ = rotZ;
    _quaternionInformation = true;
    _q0 = q0;
    _q1 = q1;
    _q2 = q2;
    _q3 = q3;
}

DM_PointOfView::DM_PointOfView(const DM_PointOfView &pof)
{
    _name = pof._name;
    _x = pof._x;
    _y = pof._y;
    _z = pof._z;
    _rotX = pof._rotX;
    _rotY = pof._rotY;
    _rotZ = pof._rotZ;
    _q0 = pof._q0;
    _quaternionInformation = pof._quaternionInformation;
    _q1 = pof._q1;
    _q2 = pof._q2;
    _q3 = pof._q3;
}

QString DM_PointOfView::toString() const
{
    return QString("%1 (x:%2 y:%3 z:%4 rx:%5 ry:%6 rz:%7)%8").arg(name())
                                                        .arg(x())
                                                        .arg(y())
                                                        .arg(z())
                                                        .arg(rotX())
                                                        .arg(rotY())
                                                        .arg(rotZ())
                                                        .arg(_quaternionInformation ? " *" : "");
}

QString DM_PointOfView::toSaveString() const
{
    return QString("%1|%2;%3;%4;%5;%6;%7;%8;%9;%10;%11;%12").arg(name())
                                                            .arg(x())
                                                            .arg(y())
                                                            .arg(z())
                                                            .arg(rotX())
                                                            .arg(rotY())
                                                            .arg(rotZ())
                                                            .arg(_quaternionInformation ? "true" : "false")
                                                            .arg(q0())
                                                            .arg(q1())
                                                            .arg(q2())
                                                            .arg(q3());
}

DM_PointOfView DM_PointOfView::loadFromString(QString saveString, bool &ok)
{
    ok = false;

    QStringList fList = saveString.split("|");

    if(fList.size() == 2)
    {
        QStringList sList = fList.at(1).split(";");
        int size = sList.size();

        if((size == 6)
                || (size == 11))
        {
            ok = true;

            QString name = fList.at(0);
            double x = sList.at(0).toDouble();
            double y = sList.at(1).toDouble();
            double z = sList.at(2).toDouble();
            double rotX = sList.at(3).toDouble();
            double rotY = sList.at(4).toDouble();
            double rotZ = sList.at(5).toDouble();

            if(size == 11)
            {
                if(sList.at(6) == "true")
                {
                    double q0 = sList.at(7).toDouble();
                    double q1 = sList.at(8).toDouble();
                    double q2 = sList.at(9).toDouble();
                    double q3 = sList.at(10).toDouble();

                    return DM_PointOfView(name, x, y, z, rotX, rotY, rotZ, q0, q1, q2, q3);
                }
            }

            return DM_PointOfView(name, x, y, z, rotX, rotY, rotZ);
        }
    }

    return DM_PointOfView();
}

bool DM_PointOfView::operator==(const DM_PointOfView &other) const
{
    return (_name == other._name)
            &&(_x == other._x)
            && (_y == other._y)
            && (_z == other._z)
            && (_rotX == other._rotX)
            && (_rotY == other._rotY)
            && (_rotZ == other._rotZ)
            && (_q0 == other._q0)
            && (_q1 == other._q1)
            && (_q2 == other._q2)
            && (_q3 == other._q3);
}
