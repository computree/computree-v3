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

#include "ct_grid3d_points.h"


CT_Grid3D_Points::CT_Grid3D_Points() : CT_AbstractGrid3D()
{
    _minCoordinates(0) = 0;
    _minCoordinates(1) = 0;
    _minCoordinates(2) = 0;
    _res = 1;
    _dimx = 1;
    _dimy = 1;
    _dimz = 1;
    _maxCoordinates(0) = 0;
    _maxCoordinates(1) = 0;
    _maxCoordinates(2) = 0;
}

CT_Grid3D_Points::~CT_Grid3D_Points()
{
    qDeleteAll(_cells.values());
    _cells.clear();
}

CT_Grid3D_Points::CT_Grid3D_Points(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            double xmin,
                            double ymin,
                            double zmin,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            double resolution) : CT_AbstractGrid3D(model, result)
{
    _minCoordinates(0) = xmin;
    _minCoordinates(1) = ymin;
    _minCoordinates(2) = zmin;

    _res = resolution;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _maxCoordinates(0) = minX() + _res * _dimx;
    _maxCoordinates(1) = minY() + _res * _dimy;
    _maxCoordinates(2) = minZ() + _res * _dimz;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);
}

CT_Grid3D_Points::CT_Grid3D_Points(const QString &modelName,
                            const CT_AbstractResult *result,
                            double xmin,
                            double ymin,
                            double zmin,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            double resolution) : CT_AbstractGrid3D(modelName, result)
{
    _minCoordinates(0) = xmin;
    _minCoordinates(1) = ymin;
    _minCoordinates(2) = zmin;

    _res = resolution;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _maxCoordinates(0) = minX() + _res * _dimx;
    _maxCoordinates(1) = minY() + _res * _dimy;
    _maxCoordinates(2) = minZ() + _res * _dimz;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);
}


CT_Grid3D_Points* CT_Grid3D_Points::createGrid3DFromXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                              const CT_AbstractResult *result,
                                                              double xmin,
                                                              double ymin,
                                                              double zmin,
                                                              double xmax,
                                                              double ymax,
                                                              double zmax,
                                                              double resolution,
                                                              bool extends)
{


    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimy = ceil((ymax - ymin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

    if (extends)
    {
        // to ensure a point exactly on a maximum limit of the grid will be included in the grid
        while (xmax >= (xmin + dimx * resolution))
        {
            dimx++;
        }

        while (ymax >= (ymin + dimy * resolution))
        {
            dimy++;
        }

        while (zmax >= (zmin + dimz * resolution))
        {
            dimz++;
        }
    }

    return new CT_Grid3D_Points(model, result, xmin, ymin, zmin, dimx, dimy, dimz, resolution);
}

CT_Grid3D_Points* CT_Grid3D_Points::createGrid3DFromXYZCoords(const QString &modelName,
                                                              const CT_AbstractResult *result,
                                                              double xmin,
                                                              double ymin,
                                                              double zmin,
                                                              double xmax,
                                                              double ymax,
                                                              double zmax,
                                                              double resolution,
                                                              bool extends)
{
    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimy = ceil((ymax - ymin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

    if (extends)
    {
        // to ensure a point exactly on a maximum limit of the grid will be included in the grid
        while (xmax >= (xmin + dimx * resolution))
        {
            dimx++;
        }

        while (ymax >= (ymin + dimy * resolution))
        {
            dimy++;
        }

        while (zmax >= (zmin + dimz * resolution))
        {
            dimz++;
        }
    }

    return new CT_Grid3D_Points(modelName, result, xmin, ymin, zmin, dimx, dimy, dimz, resolution);
}


bool CT_Grid3D_Points::addPoint(size_t pointGlobalIndex)
{
    const CT_Point& point = _pointAccessor.constPointAt(pointGlobalIndex);

    size_t cellIndex;
    if (this->indexAtXYZ(point(0), point(1), point(2), cellIndex))
    {
        if (_cells.contains(cellIndex))
        {
            (_cells[cellIndex])->append(pointGlobalIndex);
        } else {
            QList<size_t>* list = new QList<size_t>();
            list->append(pointGlobalIndex);
            _cells.insert(cellIndex, list);
        }
    } else {
        return false;
    }
    return true;
}

bool CT_Grid3D_Points::addPoint(size_t pointLocalIndex, double x, double y, double z)
{
    size_t cellIndex;
    if (this->indexAtXYZ(x, y, z, cellIndex))
    {
        if (_cells.contains(cellIndex))
        {
            (_cells[cellIndex])->append(pointLocalIndex);
        } else {
            QList<size_t>* list = new QList<size_t>();
            list->append(pointLocalIndex);
            _cells.insert(cellIndex, list);
        }
    } else {
        return false;
    }
    return true;
}

bool CT_Grid3D_Points::addPointAtIndex(size_t cellIndex, size_t pointLocalIndex)
{
    if (!(cellIndex >= this->nCells())) {return false;}

    if (_cells.contains(cellIndex))
    {
        (_cells[cellIndex])->append(pointLocalIndex);
    } else {
        QList<size_t>* list = new QList<size_t>();
        list->append(pointLocalIndex);
        _cells.insert(cellIndex, list);
    }

    return true;
}



const QList<size_t> *CT_Grid3D_Points::getConstPointIndexList(size_t cellIndex) const
{
    if (_cells.contains(cellIndex))
    {
        return _cells.value(cellIndex);
    }
    return &_emptyList;
}

QList<size_t> CT_Grid3D_Points::getCellIndicesAtNeighbourhoodN(size_t originIndex, size_t n) const
{
    QList<size_t> indices;

    if (n == 0)
    {
        indices.append(originIndex);
    } else {

        size_t lin, col, levz;
        this->indexToGrid(originIndex, col, lin, levz);


        // Upper plane
        size_t neighbLevz = levz + n;
        if (neighbLevz < this->_dimz)
        {
            for (size_t xx = col - n ; xx <= col + n ; xx++)
            {
                if (xx < this->_dimx)
                {
                    for (size_t yy = lin - n ; yy <= lin + n ; yy++)
                    {
                        size_t neighbIndex;

                        if (this->index(xx, yy, neighbLevz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }
            }
        }

        // Lower plane
        neighbLevz = levz - n;
        if (neighbLevz < this->_dimz)
        {
            for (size_t xx = col - n ; xx <= col + n ; xx++)
            {
                if (xx < this->_dimx)
                {
                    for (size_t yy = lin - n ; yy <= lin + n ; yy++)
                    {
                        size_t neighbIndex;
                        if (this->index(xx, yy, neighbLevz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }
            }
        }

        // other levels
        for (size_t zz = levz - n + 1 ; zz <= levz + n - 1 ; zz++)
        {
            if (zz < this->_dimz)
            {
                size_t yval = lin - n;
                if (yval < this->_dimy)
                {
                    for (size_t xx = col - n ; xx <= col + n ; xx++)
                    {
                        size_t neighbIndex;
                        if (this->index(xx, yval, zz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }

                yval = lin + n;
                if (yval < this->_dimy)
                {
                    for (size_t xx = col - n ; xx <= col + n ; xx++)
                    {
                        size_t neighbIndex;
                        if (this->index(xx, yval, zz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }

                size_t xval = col - n;
                if (xval < this->_dimx)
                {
                    for (size_t yy = lin - n + 1; yy <= lin + n - 1; yy++)
                    {
                        size_t neighbIndex;
                        if (this->index(xval, yy, zz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }

                xval = col + n;
                if (xval < this->_dimx)
                {
                    for (size_t yy = lin - n + 1; yy <= lin + n - 1; yy++)
                    {
                        size_t neighbIndex;
                        if (this->index(xval, yy, zz, neighbIndex))
                        {
                            indices.append(neighbIndex);
                        }
                    }
                }
            }
        }
    }

    return indices;
}

size_t CT_Grid3D_Points::getPointsIndicesInsideSphere(size_t gridIndex, double radius, QList<size_t> *indexList) const
{
    // point number
    size_t n = 0;
    double radius2 = radius*radius;

    // center of reference cell (center of the sphere)
    Eigen::Vector3d refCenter;
    Eigen::Vector3d currentCenter;
    size_t cellIndex;
    if (this->getCellCenterCoordinates(gridIndex, refCenter))
    {
        // Compute bounding box for search
        size_t minXcol, maxXcol, minYlin, maxYlin, minZlev, maxZlev;
        if (!this->colX(refCenter(0) - radius, minXcol)) {minXcol = 0;}
        if (!this->colX(refCenter(0) + radius, maxXcol)) {maxXcol = this->xdim() - 1;}
        if (!this->linY(refCenter(1) - radius, minYlin)) {minYlin = 0;}
        if (!this->linY(refCenter(1) + radius, maxYlin)) {maxYlin = this->ydim() - 1;}
        if (!this->levelZ(refCenter(2) - radius, minZlev)) {minZlev = 0;}
        if (!this->levelZ(refCenter(2) + radius, maxZlev)) {maxZlev = this->zdim() - 1;}

        for (size_t xx = minXcol ; xx <= maxXcol ; xx++)
        {
            for (size_t yy = minYlin ; yy <= maxYlin ; yy++)
            {
                for (size_t zz = minZlev ; zz <= maxZlev ; zz++)
                {
                    if (this->index(xx, yy, zz, cellIndex))
                    {
                        if (this->getCellCenterCoordinates(cellIndex, currentCenter))
                        {
                            double dist2 = pow(currentCenter(0) - refCenter(0), 2) + pow(currentCenter(1) - refCenter(1), 2) + pow(currentCenter(2) - refCenter(2), 2);

                            if (dist2 <= radius2)
                            {
                                const QList<size_t> *indices = this->getConstPointIndexList(cellIndex);
                                n += indices->size();

                                if (indexList != NULL)
                                {
                                    if (indices->size() > 0)
                                    {
                                        indexList->append(*indices);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return n;
}

size_t CT_Grid3D_Points::getPointIndicesIncludingKNearestNeighbours(Eigen::Vector3d position, size_t k, double maxDist, QList<size_t> &indexList) const
{
    size_t n = 0;

    size_t index;
    if (this->indexAtXYZ(position(0), position(1), position(2), index))
    {
        double radius = this->resolution() / 100.0;

        while (n < k && radius < maxDist)
        {
            n = this->getPointsIndicesInsideSphere(index, radius, NULL);
            radius += this->resolution();
        }

        n = this->getPointsIndicesInsideSphere(index, radius, &indexList);
    }

    return n;
}



CT_AbstractItemDrawable* CT_Grid3D_Points::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Grid3D_Points* cpy = new CT_Grid3D_Points((const CT_OutAbstractSingularItemModel *)model, result, minX(), minY(), minZ(), _dimx, _dimy, _dimz, _res);

    cpy->setId(id());
    cpy->setAlternativeDrawManager(getAlternativeDrawManager());

    return cpy;
}

