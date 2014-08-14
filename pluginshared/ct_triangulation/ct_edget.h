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

#ifndef CT_EDGET_H
#define CT_EDGET_H

#include "pluginShared_global.h"

#include <QSharedPointer>

class CT_NodeT;
class CT_TriangleT;

class PLUGINSHAREDSHARED_EXPORT CT_EdgeT
{
public:

    static QSharedPointer<CT_EdgeT> create(QSharedPointer<CT_NodeT> n1, QSharedPointer<CT_NodeT> n2)
    {
        QSharedPointer<CT_EdgeT> ptr(new CT_EdgeT());
        ptr.data()->_this = ptr.toWeakRef();
        ptr.data()->update(n1, n2);

        return ptr;
    }

    // retourne le premier point (node) de l'arete
    QSharedPointer<CT_NodeT> getN1() const;
    // retourne le deuxieme point (node) de l'arete
    QSharedPointer<CT_NodeT> getN2() const;
    // retourne la meme arete que celle-ci mais inversee
    QSharedPointer<CT_EdgeT> getInversedEdge() const;
    // retourne l'arete a gauche de cette arete (sens anti-horaire)
    QSharedPointer<CT_EdgeT> getLeftEdge() const;
    // retourne l'arete de bord si elle existe
    QSharedPointer<CT_EdgeT> getHullEdge() const;
    QSharedPointer<CT_EdgeT> getMostLeft() const;
    QSharedPointer<CT_EdgeT> getMostRight() const;
    // retourne le triangle associe a l'arete (pour avoir le triangle de l'autre cote de l'arete, utiliser l'arete inversee : getInversedEdge())
    QSharedPointer<CT_TriangleT> getTriangle() const;

    // a ne pas utiliser :
    void update(QSharedPointer<CT_NodeT> n1, QSharedPointer<CT_NodeT> n2);

    void setInversedEdge(QSharedPointer<CT_EdgeT> inversed_edge);
    void setLeftEdge(QSharedPointer<CT_EdgeT> next_edge);
    void setHullEdge(QSharedPointer<CT_EdgeT> next_edge_h);
    void setTriangle(QSharedPointer<CT_TriangleT> triangle);

    QSharedPointer<CT_EdgeT> makeSymmetry();
    void linkSymmetry(QSharedPointer<CT_EdgeT> inversed_edge);

    int onSide(QSharedPointer<CT_NodeT> n) const;
    void setN1Edge();           // affecte a la Node n1 cette arete

    bool contains(const QSharedPointer<CT_NodeT> &node) const;

private:

    CT_EdgeT();

    QWeakPointer<CT_EdgeT>         _this;

    QSharedPointer<CT_NodeT>       _n1;            // point de depart de l'arete
    QSharedPointer<CT_NodeT>       _n2;            // point de fin de l'arete
    QSharedPointer<CT_EdgeT>       _inversed_edge; // cette arete mais inversee
    QSharedPointer<CT_EdgeT>       _next_edge;     // arete suivante dans le triangle, dans le sens anti-trigonometrique
    QSharedPointer<CT_EdgeT>       _next_edge_h;   // lien vers l'enveloppe convexe
    QSharedPointer<CT_TriangleT> _triangle;      // triangle contenant l'arete
    double                      _a, _b, _c;     // parametre de l'equation de la droite aX+bY+c=0

    void computeLineEquation(); // calcul les parametres de l'equation de la droite
};

#endif // CT_EDGET_H
