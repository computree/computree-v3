/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
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

#ifndef CT_GRID3D_HPP
#define CT_GRID3D_HPP

#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardgrid3ddrawmanager.h"

#include <math.h>
#include <typeinfo>
#include <limits>

#include "ct_math/ct_math.h"
#include "qdebug.h"

template< typename DataT> CT_DEFAULT_IA_INIT(CT_Grid3D<DataT>)

template< typename DataT>
const CT_StandardGrid3DDrawManager<DataT> CT_Grid3D<DataT>::ABSGRID3D_DRAW_MANAGER;

template< typename DataT>
CT_Grid3D<DataT>::CT_Grid3D() : CT_AbstractGrid3D()
{
    _minCoordinates.setX(0);
    _minCoordinates.setY(0);
    _minCoordinates.setZ(0);
    _res = 1;
    _dimx = 0;
    _dimy = 0;
    _dimz = 0;
    _maxCoordinates.setX(0);
    _maxCoordinates.setY(0);
    _maxCoordinates.setZ(0);
    _NAdata = -1;
    _dataMax = -1;
    _dataMin = -1;

    setBaseDrawManager(&ABSGRID3D_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D<DataT>::CT_Grid3D(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            float xmin,
                            float ymin,
                            float zmin,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            float resolution,
                            DataT na,
                            DataT initValue) : CT_AbstractGrid3D(model, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);

    _res = resolution;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _maxCoordinates.setX(minX() + _res * _dimx);
    _maxCoordinates.setY(minY() + _res * _dimy);
    _maxCoordinates.setZ(minZ() + _res * _dimz);

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());
    initGridWithValue(initValue);

    setBaseDrawManager(&ABSGRID3D_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D<DataT>::CT_Grid3D(const QString &modelName,
                            const CT_AbstractResult *result,
                            float xmin,
                            float ymin,
                            float zmin,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            float resolution,
                            DataT na,
                            DataT initValue) : CT_AbstractGrid3D(modelName, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);

    _res = resolution;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _maxCoordinates.setX(minX() + _res * _dimx);
    _maxCoordinates.setY(minY() + _res * _dimy);
    _maxCoordinates.setZ(minZ() + _res * _dimz);

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());
    initGridWithValue(initValue);

    setBaseDrawManager(&ABSGRID3D_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D<DataT>* CT_Grid3D<DataT>::createGrid3DFromXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                              const CT_AbstractResult *result,
                                                              float xmin,
                                                              float ymin,
                                                              float zmin,
                                                              float xmax,
                                                              float ymax,
                                                              float zmax,
                                                              float resolution,
                                                              DataT na,
                                                              DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimy = ceil((ymax - ymin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

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

    return new CT_Grid3D<DataT>(model, result, xmin, ymin, zmin, dimx, dimy, dimz, resolution, na, initValue);
}

template< typename DataT>
CT_Grid3D<DataT>* CT_Grid3D<DataT>::createGrid3DFromXYZCoords(const QString &modelName,
                                                              const CT_AbstractResult *result,
                                                              float xmin,
                                                              float ymin,
                                                              float zmin,
                                                              float xmax,
                                                              float ymax,
                                                              float zmax,
                                                              float resolution,
                                                              DataT na,
                                                              DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);
    size_t dimy = ceil((ymax - ymin)/resolution);
    size_t dimz = ceil((zmax - zmin)/resolution);

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

    return new CT_Grid3D<DataT>(modelName, result, xmin, ymin, zmin, dimx, dimy, dimz, resolution, na, initValue);
}


template< typename DataT>
CT_Grid3D<DataT>::~CT_Grid3D()
{
    _data.clear();
}

template< typename DataT>
void CT_Grid3D<DataT>::initGridWithValue(const DataT val)
{
    for (size_t  i = 0 ; i < nCells() ; i++)
    {
        _data[i] = val;
    }
    _dataMin = val;
    _dataMax = val;
}


template< typename DataT>
void CT_Grid3D<DataT>::setValueAtIndexFromDouble(const size_t &index, const double &value)
{
    setValueAtIndex(index, (DataT) value);
}


template< typename DataT>
void CT_Grid3D<DataT>::computeMinMax()
{
    size_t ncells = nCells();
    if (ncells > 0) {
        _dataMin = _data[0];
        _dataMax = _data[0];

        for (size_t i = 1 ; i < ncells ; i++)
        {
            if (_data[i] != NA())
            {
                if (_dataMax==NA() || _data[i] > _dataMax) {_dataMax = _data[i];}
                if (_dataMin==NA() || _data[i] < _dataMin) {_dataMin = _data[i];}
            }
        }
    }
}



template< typename DataT>
QString CT_Grid3D<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Grid3D<DataT>::staticGetType()
{
    return CT_AbstractGrid3D::staticGetType() + "/CT_Grid3D<" + typeid(DataT).name() + ">";
}

template< typename DataT>
QString CT_Grid3D<DataT>::name() const
{
    return QString("CT_Grid3D<") + typeid(DataT).name() + QString(">");
}

template< typename DataT>
bool CT_Grid3D<DataT>::setValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}
    _data[index] = value;

    return true;
}

template< typename DataT>
bool CT_Grid3D<DataT>::setValue(const size_t colx, const size_t liny, const size_t levz, const DataT value)
{
    size_t i;
    if (index(colx, liny, levz, i))
    {
        return setValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D<DataT>::setValueAtXYZ(const float x, const float y, const float z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setValueAtIndex(i, value);
    }
    return false;
}


template< typename DataT>
DataT CT_Grid3D<DataT>::valueAtIndex(const size_t index) const
{
    if (index >= nCells()) {return NA();}
    return _data[index];
}

template< typename DataT>
float CT_Grid3D<DataT>::ratioValueAtIndex(const size_t index) const
{
    if (_dataMax <= _dataMin) {return 1;}
    DataT value = valueAtIndex(index);
    if (value == NA()) {return -1;}
    return (float) (((double)(value - _dataMin))/((double)(_dataMax - _dataMin)));
}

template< typename DataT>
double CT_Grid3D<DataT>::valueAtIndexAsDouble(const size_t index) const
{
    DataT value = valueAtIndex(index);
    if (value == NA()) {return std::numeric_limits<DataT>::quiet_NaN();}
    return value;
}

template< typename DataT>
QString CT_Grid3D<DataT>::valueAtIndexAsString(const size_t index) const
{
    DataT value = valueAtIndex(index);
    return QString::number(value);
}

template< typename DataT>
QString CT_Grid3D<DataT>::NAAsString() const
{
    return QString::number(NA());
}

template< typename DataT>
DataT CT_Grid3D<DataT>::value(const size_t colx, const size_t liny, const size_t levz) const
{
    size_t i;
    if (index(colx, liny, levz, i))
    {
        return valueAtIndex(i);
    }
    return NA();
}

template< typename DataT>
DataT CT_Grid3D<DataT>::dataFromArray(const size_t &x, const size_t &y, const size_t &z) const
{
    size_t i;
    if (index(x, y, z, i))
    {
        return valueAtIndex(i);
    }
    return NA();
}

template< typename DataT>
DataT CT_Grid3D<DataT>::dataFromArray(const size_t &index) const
{
    if (index >= nCells()) {return NA();}
    return _data[index];
}

template< typename DataT>
DataT CT_Grid3D<DataT>::valueAtXYZ(const float x, const float y, const float z) const
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return valueAtIndex(i);
    }
    return false;
}


template< typename DataT>
bool CT_Grid3D<DataT>::setMaxValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if ((currentValue == NA()) || (value > currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D<DataT>::setMaxValueAtXYZ(const float x, const float y, const float z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setMaxValueAtIndex(i, value);
    }
    return false;
}


template< typename DataT>
bool CT_Grid3D<DataT>::setMinValueAtIndex(const size_t index, DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if ((currentValue == NA()) || (value < currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D<DataT>::setMinValueAtXYZ(const float x, const float y, const float z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setMinValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D<DataT>::addValueAtIndex(const size_t index, DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data[index];

    if (currentValue == NA())
    {
        return setValueAtIndex(index, value);
    } else if (value != NA())
    {
        return setValueAtIndex(index, value + currentValue);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D<DataT>::addValueAtXYZ(const float x, const float y, const float z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return addValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
QList<DataT> CT_Grid3D<DataT>::neighboursValues(const size_t colx, const size_t liny, const size_t levz, const size_t distance, const bool keepNAs, const CenterMode centermode) const
{
    QList<DataT> liste;

    if (distance < 1) {return liste;}
    if (colx >= _dimx) {return liste;}
    if (liny >= _dimy) {return liste;}
    if (levz >= _dimz) {return liste;}

    size_t firstColx = colx-distance;
    size_t lastColx = colx+distance;
    size_t firstLiny = liny-distance;
    size_t lastLiny = liny+distance;
    size_t firstLinz = levz-distance;
    size_t lastLinz = levz+distance;

    if (firstColx >= _dimx) {firstColx = 0;}
    if (lastColx >= _dimx) {lastColx = _dimx - 1;}
    if (firstLiny >= _dimy) {firstLiny = 0;}
    if (lastLiny >= _dimy) {lastLiny = _dimy - 1;}
    if (firstLinz >= _dimz) {firstLinz = 0;}
    if (lastLinz >= _dimz) {lastLinz = _dimz - 1;}


    for (size_t xx = firstColx ; xx <= lastColx ; xx++)
    {
        for (size_t yy = firstLiny ; yy <= lastLiny ; yy++)
        {
            for (size_t zz = firstLinz ; zz <= lastLinz ; zz++)
            {

                DataT val = value(xx, yy, zz);
                if ((xx == colx) && (yy == liny) && (zz == levz)) {
                    if (centermode == CM_KeepCenter) {
                        if ((val != NA()) || keepNAs) {liste.append(val);}
                    } else if (centermode == CM_NAasCenter) {
                        liste.append(NA());
                    }
                } else {
                    if ((val != NA()) || keepNAs) {liste.append(val);}
                }
            }
        }
    }

    return liste;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid3D<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid3D<DataT>* cpy = new CT_Grid3D<DataT>((const CT_OutAbstractSingularItemModel *)model, result, minX(), minY(), minZ(), _dimx, _dimy, _dimz, _res, _NAdata, _NAdata);
    cpy->setId(id());

    for (size_t i = 0 ; i < nCells() ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if (nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(getAlternativeDrawManager());

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid3D<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid3D<DataT>* cpy = new CT_Grid3D<DataT>(modelName, result, minX(), minY(), minZ(), _dimx, _dimy, _dimz, _res, _NAdata, _NAdata);
    cpy->setId(id());

    for (size_t i = 0 ; i < nCells() ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if (nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(getAlternativeDrawManager());

    return cpy;
}

#endif // CT_GRID3D_HPP
