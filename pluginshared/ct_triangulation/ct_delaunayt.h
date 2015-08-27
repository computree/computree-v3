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

#ifndef CT_DELAUNAYT_H
#define CT_DELAUNAYT_H

#include "pluginShared_global.h"

#include <QSharedPointer>

class CT_NodeT;
class CT_EdgeT;
class CT_TriangleT;

class PLUGINSHAREDSHARED_EXPORT CT_DelaunayT
{
public:
    CT_DelaunayT();
    ~CT_DelaunayT();

    // supprimer tous les points ajoutes a la liste des points et efface les informations de la triangulation
    void clear();

    // insertion d'un point (on peut inserer un point, retrouver la liste de ses voisins en appelant
    // la methode getNodesAround(), puis supprimer ce point de la triangulation en appelant removeNode(...). Lorsque
    // le point est supprime, il ne connait plus la liste de ses voisins)
    void insertNode(QSharedPointer<CT_NodeT> n, bool debug = false);
    // supression d'un point (node)
    void removeNode(QSharedPointer<CT_NodeT> n);

    // retourne la liste de tous les points ajoutes a la triangulation
    const QList< QSharedPointer<CT_NodeT> >& getNodeList() const;
    // retourne la liste de toutes les aretes trouvees
    const QList< QSharedPointer<CT_EdgeT> >& getEdgeList() const;
    // retourne la liste de tous les triangles trouvees
    const QList< QSharedPointer<CT_TriangleT> >& getTriangleList() const;

    // retourne le point de depart de l'enveloppe convexe (pour continuer il faut appeler la methode getHullEdge() recursivement)
    QSharedPointer<CT_EdgeT> getHullEdgeStart() const;
    // retourne la liste des points (Node) de l'enveloppe convexe
    QList< QSharedPointer<CT_NodeT> > getHullNodes() const;

    CT_DelaunayT* copy();

private:

    QList< QSharedPointer<CT_NodeT> >      _list_nodes;
    QList< QSharedPointer<CT_EdgeT> >      _list_edges;
    QList< QSharedPointer<CT_TriangleT> >  _list_triangles;
    QSharedPointer<CT_EdgeT>               _hull_edge_start;
    QSharedPointer<CT_EdgeT>               _act_edge;

    void expandTri(QSharedPointer<CT_EdgeT> e, QSharedPointer<CT_NodeT> nd, int type);
    void expandHull(QSharedPointer<CT_NodeT> nd);
    int searchEdge(QSharedPointer<CT_EdgeT> e, QSharedPointer<CT_NodeT> nd);
    void swapTest(QSharedPointer<CT_EdgeT> e11);
    bool nodeInTriangleTest(QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3, QSharedPointer<CT_NodeT> n);
};

#endif // CT_DELAUNAYT_H
