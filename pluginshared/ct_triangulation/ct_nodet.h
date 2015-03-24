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

#ifndef CT_NODET_H
#define CT_NODET_H

#include <QSharedPointer>
#include "ct_edget.h"

#include "eigen/Eigen/Core"

class CT_VoronoiNodeT;

class PLUGINSHAREDSHARED_EXPORT CT_NodeT
{
public:
    static QSharedPointer<CT_NodeT> create(Eigen::Vector3d *point, QObject *associate_object = NULL, bool toBeDeleted = false)
    {
        QSharedPointer<CT_NodeT> ptr(new CT_NodeT(point, associate_object, toBeDeleted));
        ptr.data()->_this = ptr.toWeakRef();

        return ptr;
    }

    ~CT_NodeT();

    void clear();

    // retourne la distance 2D entre cette node et le point passe en parametre
    double distance(double x, double y) const;

    // retourne une des aretes reliees a cette node
    QSharedPointer<CT_EdgeT> getEdge() const;
    // retourne le tableau des coordonnees de cette node x, y ,z
    Eigen::Vector3d* getPoint() const;
    // retourne l'objet associe
    QObject* getAssociateObject() const;
    // retourne la liste des nodes autour reliees a cette node
    QList< QSharedPointer<CT_NodeT> > getNodesAround() const;
    // retourne les points de voronoi de cette node (centre des cercles des triangles autour de cette node)
    QList< QSharedPointer<CT_VoronoiNodeT> > getVoronoi() const;

    // a ne pas utiliser :
    void setEdge(QSharedPointer<CT_EdgeT> edge);

private:

    CT_NodeT(Eigen::Vector3d *point, QObject *associate_object = NULL, bool toBeDeleted = false);

    QWeakPointer<CT_NodeT>     _this;              // pointeur de la node

    Eigen::Vector3d                   *_point;            // coordonnee X, Y et Z (Z pouvant etre omis pour une triangulation 2D)
    QSharedPointer<CT_EdgeT>   _edge;              // arete qui part de ce noeud
    QObject                 *_associate_object; // objet associe
    bool                    _deletePoint; // faut-il supprimer le point attaché
};

#endif // CT_NODET_H
