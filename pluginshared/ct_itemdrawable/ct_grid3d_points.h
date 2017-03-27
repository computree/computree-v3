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

#ifndef CT_GRID3D_POINTS_H
#define CT_GRID3D_POINTS_H

#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_accessor/ct_pointaccessor.h"


class PLUGINSHAREDSHARED_EXPORT CT_Grid3D_Points : public CT_AbstractGrid3D
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Grid3D_Points, CT_AbstractGrid3D, Point 3D Grid)

public:

    CT_Grid3D_Points();

    ~CT_Grid3D_Points();


    /*!
     * \brief Contructor with integer column and row coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param dimx Number of colums
     * \param dimy Number of rows
     * \param dimz Number of zlevels
     * \param resolution Size of a cell
     */
    CT_Grid3D_Points(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resolution);

    CT_Grid3D_Points(const QString &modelName,
              const CT_AbstractResult *result,
              double xmin,
              double ymin,
              double zmin,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resolution);

    /*!
     * \brief Factory with min and max (X,Y,Z) coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param xmax Maximum X coordinate (upper right corner)
     * \param ymax Maximum Y coordinate (upper right corner)
     * \param zmax Maximum Z coordinate (upper right corner)
     * \param resolution Size of a cell
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    static CT_Grid3D_Points* createGrid3DFromXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                          const CT_AbstractResult *result,
                                                          double xmin,
                                                          double ymin,
                                                          double zmin,
                                                          double xmax,
                                                          double ymax,
                                                          double zmax,
                                                          double resolution,
                                                          bool extends = true);

    static CT_Grid3D_Points* createGrid3DFromXYZCoords(const QString &modelName,
                                                          const CT_AbstractResult *result,
                                                          double xmin,
                                                          double ymin,
                                                          double zmin,
                                                          double xmax,
                                                          double ymax,
                                                          double zmax,
                                                          double resolution,
                                                          bool extends = true);


    bool addPoint(size_t pointGlobalIndex);
    bool addPoint(size_t pointLocalIndex, double x, double y, double z);
    bool addPointAtIndex(size_t cellIndex, size_t pointLocalIndex);

    const QList<size_t>* getConstPointIndexList(size_t cellIndex) const;

    QList<size_t> getCellIndicesAtNeighbourhoodN(size_t originIndex, size_t n) const;

    size_t getPointsIndicesInsideSphere(size_t gridIndex, double radius, QList<size_t> *indexList) const;

    size_t getPointIndicesIncludingKNearestNeighbours(Eigen::Vector3d position, size_t k, double maxDist, QList<size_t> &indexList) const;

    void getIndicesWithPoints(QList<size_t> &list) const {list.append(_cells.keys());}


    // Neutralize useless Methods
    virtual double ratioValueAtIndex(const size_t index) const {Q_UNUSED(index); qDebug() << "This method should not be used in this context";return 0;}
    virtual double valueAtIndexAsDouble(const size_t index) const {Q_UNUSED(index); qDebug() << "This method should not be used in this context";return 0;}
    virtual QString NAAsString() const {qDebug() << "This method should not be used in this context";return 0;}
    virtual QString valueAtIndexAsString(const size_t index) const {Q_UNUSED(index); qDebug() << "This method should not be used in this context";return 0;}
    virtual void computeMinMax() {qDebug() << "This method should not be used in this context";}
    virtual void setValueAtIndexFromDouble(const size_t &index, const double &value) {Q_UNUSED(index); Q_UNUSED(value); qDebug() << "This method should not be used in this context";}


    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    QMap<size_t, QList<size_t>* >    _cells;

    QList<size_t>                   _emptyList;
    CT_PointAccessor                _pointAccessor;


};

#endif // CT_GRID3D_POINTS_H
