/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#ifndef CT_REFERENCEPOINT_H
#define CT_REFERENCEPOINT_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardreferencepointdrawmanager.h"

/*!
 * \class CT_ReferencePoint
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable representing reference point</b>
 *
 * It is defined by (X,Y,Z) coordinates.
 * Moreover it is referenced by a unique refId automatically generated at creation.
 * If the item is copied, the copy will have the same refId than the original.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_ReferencePoint : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_ReferencePoint, CT_AbstractItemDrawableWithoutPointCloud)

public:
    CT_ReferencePoint();

    /**
      * \brief Contructeur
      */
    CT_ReferencePoint(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            double x, double y, double z, double buffer);

    CT_ReferencePoint(const QString &modelName,
                            const CT_AbstractResult *result,
                            double x, double y, double z, double buffer);

    virtual QString name() const;


    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    inline double x() const { return getCenterX(); }
    inline double y() const { return getCenterY(); }
    inline double z() const { return getCenterZ(); }
    inline double refId() const { return _refId; }

    inline double xyBuffer() const { return _xyBuffer;}

    void setXYBuffer(double buffer);

    // neutralisation des méthodes permettant de modifier les (x,y,z)
    // Une fois créés (constructeurs) : ne sont plus modifiables
    void setCenterX(double x);
    void setCenterY(double y);
    void setCenterZ(double z);
    void setCenterCoordinate(const Eigen::Vector3d& center);

private:

    const static CT_StandardReferencePointDrawManager  REFERENCEPOINT_DRAW_MANAGER;

    static int      _currentRefId;

    double           _xyBuffer;
    int             _refId;         /*!< Identifiant automatique unique à la construction, identique à la copie */


    // Contructeur privé pour la copie (permettant de conserver le refId de l'instance copiée
    CT_ReferencePoint(const CT_OutAbstractSingularItemModel *model,
                      const CT_AbstractResult *result,
                      double x, double y, double z, double buffer, int refId);

    CT_ReferencePoint(const QString &modelName,
                      const CT_AbstractResult  *result,
                      double x, double y, double z, double buffer, int refId);

};

#endif // CT_REFERENCEPOINT_H
