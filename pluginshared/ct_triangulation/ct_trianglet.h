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

#ifndef CT_TRIANGLET_H
#define CT_TRIANGLET_H

#include <QSharedPointer>
#include "ct_edget.h"

class PLUGINSHAREDSHARED_EXPORT CT_TriangleT
{
public:

    static QSharedPointer<CT_TriangleT> create(QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3)
    {
        QSharedPointer<CT_TriangleT> ptr(new CT_TriangleT());
        ptr.data()->_this = ptr.toWeakRef();

        ptr.data()->update(e1, e2, e3);

        return ptr;
    }

    static QSharedPointer<CT_TriangleT> create(QList< QSharedPointer<CT_EdgeT> > &list, QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3)
    {
        QSharedPointer<CT_TriangleT> ptr(new CT_TriangleT());
        ptr.data()->_this = ptr.toWeakRef();

        ptr.data()->update(e1, e2, e3);

        list.append(e1);
        list.append(e2);
        list.append(e3);

        return ptr;
    }

    // retourne une des aretes du triangle (pour avoir les autres aretes il faut appeler la methode getLeftEdge())
    QSharedPointer<CT_EdgeT> getAnEdge() const;

    // retourne le centre et rayon du cercle circonscrit du triangle
    double getCx() const;
    double getCy() const;
    double getCz() const;
    double getCRadius() const;

    // a ne pas utiliser :
    bool isInCircle(QSharedPointer<CT_NodeT> n) const;

    void removeEdgesIn(QList< QSharedPointer<CT_EdgeT> > &list) const;

    void update(QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3);

    bool contains(const QSharedPointer<CT_NodeT> &node) const;

private:

    CT_TriangleT();

    QWeakPointer<CT_TriangleT> _this;

    QSharedPointer<CT_EdgeT>   _an_edge;       // une des aretes du triangle
    double                  _circle_cx;     // coordonnee x du centre du cercle circonscrit
    double                  _circle_cy;     // coordonnee y du centre du cercle circonscrit
    double                  _circle_cz;     // coordonnee z du centre du cercle circonscrit
    double                  _circle_radius; // rayon du cercle circonscrit

    void findCircle();
};

#endif // CT_TRIANGLET_H
