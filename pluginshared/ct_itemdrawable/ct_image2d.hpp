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

#ifndef CT_IMAGE2D_HPP
#define CT_IMAGE2D_HPP

#include "ct_itemdrawable/ct_image2d.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardimage2ddrawmanager.h"
#include "opencv2/core/core.hpp"

#include <math.h>
#include <typeinfo>
#include <limits>

#include "qdebug.h"

template< typename DataT>
const CT_StandardImage2DDrawManager<DataT> CT_Image2D<DataT>::IMAGE2D_DRAW_MANAGER;

template< typename DataT>
CT_Image2D<DataT>::CT_Image2D() : CT_AbstractImage2D()
{
    _minCoordinates(0) = 0;
    _minCoordinates(1) = 0;
    _minCoordinates(2) = 0;
    _res = 1;
    _dimCol = 0;
    _dimLin = 0;
    _maxCoordinates(0) = 0;
    _maxCoordinates(1) = 0;
    _maxCoordinates(2) = 0;

    _minColCoord = 0;
    _minLinCoord = 0;

    _level = 0;

    this->setBaseDrawManager(&IMAGE2D_DRAW_MANAGER);
}

template< typename DataT>
CT_Image2D<DataT>::CT_Image2D(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_AbstractImage2D(model, result)
{
    _minCoordinates(0) = 0;
    _minCoordinates(1) = 0;
    _minCoordinates(2) = 0;
    _res = 1;
    _dimCol = 0;
    _dimLin = 0;
    _maxCoordinates(0) = 0;
    _maxCoordinates(1) = 0;
    _maxCoordinates(2) = 0;

    _minColCoord = 0;
    _minLinCoord = 0;

    _level = 0;

    setBaseDrawManager(&IMAGE2D_DRAW_MANAGER);
}

template< typename DataT>
CT_Image2D<DataT>::CT_Image2D(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractImage2D(modelName, result)
{
    _minCoordinates(0) = 0;
    _minCoordinates(1) = 0;
    _minCoordinates(2) = 0;
    _res = 1;
    _dimCol = 0;
    _dimLin = 0;
    _maxCoordinates(0) = 0;
    _maxCoordinates(1) = 0;
    _maxCoordinates(2) = 0;

    _minColCoord = 0;
    _minLinCoord = 0;

    _level = 0;

    setBaseDrawManager(&IMAGE2D_DRAW_MANAGER);
}

template< typename DataT>
CT_Image2D<DataT>::CT_Image2D(const CT_OutAbstractSingularItemModel *model,
                                const CT_AbstractResult *result,
                                double xmin,
                                double ymin,
                                size_t dimx,
                                size_t dimy,
                                double resolution,
                                double zlevel,
                                DataT na,
                                DataT initValue) : CT_AbstractImage2D(model, result)
{
    this->_minCoordinates(0) = xmin;
    this->_minCoordinates(1) = ymin;
    this->_minCoordinates(2) = zlevel;
    this->_res = resolution;
    this->_dimCol = dimx;
    this->_dimLin = dimy;
    this->_maxCoordinates(0) = this->minX() + this->_res * this->_dimCol;
    this->_maxCoordinates(1) = this->minY() + this->_res * this->_dimLin;
    this->_maxCoordinates(2) = zlevel;

    this->_minColCoord = xmin;
    this->_minLinCoord = ymin;

    this->_level = zlevel;
    this->_NAdata = na;

    this->setCenterX (this->minX() + (this->maxX() - this->minX())/2.0);
    this->setCenterY (this->minY() + (this->maxY() - this->minY())/2.0);
    this->setCenterZ (this->_level);

    this->_data = cv::Mat_<DataT>(this->_dimLin, this->_dimCol);
    initGridWithValue(initValue);
    this->setBaseDrawManager(&IMAGE2D_DRAW_MANAGER);
}



template< typename DataT>
CT_Image2D<DataT>::CT_Image2D(const QString &modelName,
                                const CT_AbstractResult *result,
                                double xmin,
                                double ymin,
                                size_t dimx,
                                size_t dimy,
                                double resolution,
                                double zlevel,
                                DataT na,
                                DataT initValue) : CT_AbstractImage2D(modelName, result)
{
    this->_minCoordinates(0) = xmin;
    this->_minCoordinates(1) = ymin;
    this->_minCoordinates(2) = zlevel;
    this->_res = resolution;
    this->_dimCol = dimx;
    this->_dimLin = dimy;
    this->_maxCoordinates(0) = this->minX() + this->_res * this->_dimCol;
    this->_maxCoordinates(1) = this->minY() + this->_res * this->_dimLin;
    this->_maxCoordinates(2) = zlevel;

    this->_minColCoord = xmin;
    this->_minLinCoord = ymin;

    this->_level = zlevel;
    this->_NAdata = na;

    this->setCenterX (this->minX() + (this->maxX() - this->minX())/2.0);
    this->setCenterY (this->minY() + (this->maxY() - this->minY())/2.0);
    this->setCenterZ (this->_level);

    this->_data = cv::Mat_<DataT>(this->_dimLin, this->_dimCol);
    initGridWithValue(initValue);

    this->setBaseDrawManager(&IMAGE2D_DRAW_MANAGER);
}

template< typename DataT>
CT_Image2D<DataT>::~CT_Image2D()
{
    _data.release();
}

template< typename DataT>
CT_Image2D<DataT>* CT_Image2D<DataT>::createImage2DFromXYCoords(const CT_OutAbstractSingularItemModel *model,
                                                                   const CT_AbstractResult *result,
                                                                   double xmin,
                                                                   double ymin,
                                                                   double xmax,
                                                                   double ymax,
                                                                   double resolution,
                                                                   double zlevel,
                                                                   DataT na,
                                                                   DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);;
    size_t dimy = ceil((ymax - ymin)/resolution);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (xmax >= (xmin + dimx * resolution))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resolution))
    {
        dimy++;
    }

    CT_Image2D<DataT>* grid = new CT_Image2D<DataT>(model, result, xmin, ymin, dimx, dimy, resolution, zlevel, na, initValue);

    return grid;
}

template< typename DataT>
CT_Image2D<DataT>* CT_Image2D<DataT>::createImage2DFromXYCoords(const QString &modelName,
                                                                   const CT_AbstractResult *result,
                                                                   double xmin,
                                                                   double ymin,
                                                                   double xmax,
                                                                   double ymax,
                                                                   double resolution,
                                                                   double zlevel,
                                                                   DataT na,
                                                                   DataT initValue)
{
    size_t dimx = ceil((xmax - xmin)/resolution);;
    size_t dimy = ceil((ymax - ymin)/resolution);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (xmax >= (xmin + dimx * resolution))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resolution))
    {
        dimy++;
    }

    CT_Image2D<DataT>* grid = new CT_Image2D<DataT>(modelName, result, xmin, ymin, dimx, dimy, resolution, zlevel, na, initValue);

    return grid;
}

template< typename DataT>
void CT_Image2D<DataT>::initGridWithValue(const DataT val)
{
    _data.setTo(cv::Scalar::all(val));
    _dataMin = val;
    _dataMax = val;
}

template< typename DataT>
void CT_Image2D<DataT>::computeMinMax()
{   
    _dataMin = NA();
    _dataMax = NA();

    cv::MatIterator_<DataT> it, end;
    for(it = _data.begin(), end = _data.end() ; it != end ; ++it)
    {
        DataT val = *it;

        if (val != NA())
        {
            if (_dataMax == NA() || val > _dataMax) {_dataMax = val;}
            if (_dataMin == NA() || val < _dataMin) {_dataMin = val;}
        }
    }
}

template< typename DataT>
QString CT_Image2D<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Image2D<DataT>::staticGetType()
{
    return CT_AbstractImage2D::staticGetType() + "/CT_Image2D<" + CT_TypeInfo::name<DataT>() + ">";
}

template< typename DataT>
QString CT_Image2D<DataT>::name() const
{
    return tr("Raster (%1)").arg(CT_TypeInfo::name<DataT>());
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Image2D<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_Image2D<DataT>* cpy = new CT_Image2D<DataT>((const CT_OutAbstractSingularItemModel *)model, result, this->minX(), this->minY(), this->_dimCol, this->_dimLin, this->_res, this->_level, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());
    for (size_t r = 0 ; r < _dimLin ; r++)
    {
        for (size_t c = 0 ; c < _dimCol ; c++)
        {
            cpy->_data(r, c) = this->_data(r,c);
        }
    }
    if (this->nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Image2D<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_Image2D<DataT>* cpy = new CT_Image2D<DataT>(modelName, result, this->minX(), this->minY(), this->_dimCol, this->_dimLin, this->_res, this->_level, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());
    for (size_t r = 0 ; r < _dimLin ; r++)
    {
        for (size_t c = 0 ; c < _dimCol ; c++)
        {
            cpy->_data(r, c) = this->_data(r,c);
        }
    }
    if (this->nCells() >0)
    {
        cpy->computeMinMax();
    }
    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}


template< typename DataT>
bool CT_Image2D<DataT>::setValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    size_t lin, col;
    indexToGrid(index, col, lin);

    return setValue(col, lin, value);
}

template< typename DataT>
bool CT_Image2D<DataT>::setValue(const size_t col, const size_t lin, const DataT value)
{
    if ((col >= _dimCol) || (lin >= _dimLin)) {return false;}
    _data(lin, col) = value;
    return true;
}

template< typename DataT>
bool CT_Image2D<DataT>::setValueAtCoords(const double x, const double y, const DataT value)
{
    size_t resultIndex;
    if (!indexAtCoords(x, y, resultIndex)) {return false;}
    return setValueAtIndex(resultIndex, value);
}


template< typename DataT>
DataT CT_Image2D<DataT>::valueAtIndex(const size_t index) const
{
    if (index >= nCells()) {return NA();}

    size_t lin, col;
    indexToGrid(index, col, lin);

    return value(col, lin);
}

template< typename DataT>
DataT CT_Image2D<DataT>::value(const size_t col, const size_t lin) const
{
    if ((col >= _dimCol) || (lin >= _dimLin)) {return NA();}
    return _data(lin, col);
}

template< typename DataT>
DataT CT_Image2D<DataT>::dataFromArray(const size_t &x, const size_t &y) const
{
    return value(y, x);
}

template< typename DataT>
DataT CT_Image2D<DataT>::dataFromArray(const size_t &index) const
{
    return valueAtIndex(index);
}

template< typename DataT>
DataT CT_Image2D<DataT>::valueAtCoords(const double x, const double y) const
{
    size_t index;
    if (!indexAtCoords(x, y, index)) {return NA();}
    return valueAtIndex(index);
}


template< typename DataT>
bool CT_Image2D<DataT>::setMaxValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    size_t lin, col;
    if (indexToGrid(index, col, lin))
    {
        DataT currentValue = _data(lin, col);

        if ((currentValue == NA()) || (value > currentValue)) {
            return setValueAtIndex(index, value);
        }
    }

    return false;
}

template< typename DataT>
bool CT_Image2D<DataT>::setMaxValueAtCoords(const double x, const double y, const DataT value)
{
    size_t indice;
    if (!indexAtCoords(x, y, indice)) {return false;}

    return setMaxValueAtIndex(indice, value);
}


template< typename DataT>
bool CT_Image2D<DataT>::setMinValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    size_t lin, col;
    if (indexToGrid(index, col, lin))
    {
        DataT currentValue = _data(lin, col);

        if ((currentValue == NA()) || (value < currentValue)) {
            return setValueAtIndex(index, value);
        }
    }

    return false;
}

template< typename DataT>
bool CT_Image2D<DataT>::setMinValueAtCoords(const double x, const double y, const DataT value)
{
    size_t indice;
    if (!indexAtCoords(x, y, indice)) {return false;}

    return setMinValueAtIndex(indice, value);
}

template< typename DataT>
bool CT_Image2D<DataT>::addValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    size_t lin, col;
    if (indexToGrid(index, col, lin))
    {
        DataT currentValue = _data(lin, col);

        if (currentValue == NA())
        {
            return setValueAtIndex(index, value);
        } else if (value != NA())
        {
            return setValueAtIndex(index, value + currentValue);
        }
    }
    return false;
}

template< typename DataT>
double CT_Image2D<DataT>::ratioValueAtIndex(const size_t index) const
{
    if (_dataMax <= _dataMin) {return 1;}
    DataT value = valueAtIndex(index);
    if (value == NA()) {return -1;}
    return (double) (((double)(value - _dataMin))/((double)(_dataMax - _dataMin)));
}

template< typename DataT>
double CT_Image2D<DataT>::valueAtIndexAsDouble(const size_t index) const
{
    DataT value = valueAtIndex(index);
    if (value == NA()) {return std::numeric_limits<DataT>::quiet_NaN();}
    return value;
}

template< typename DataT>
QString CT_Image2D<DataT>::valueAtIndexAsString(const size_t index) const
{
    DataT value = valueAtIndex(index);
    return QVariant(value).toString();
}

template< typename DataT>
QString CT_Image2D<DataT>::NAAsString() const
{
    return QVariant(NA()).toString();
}

template< typename DataT>
double CT_Image2D<DataT>::NAAsDouble() const
{
    return NA();
}

template< typename DataT>
bool CT_Image2D<DataT>::addValueAtCoords(const double x, const double y, const DataT value)
{
    size_t indice;
    if (!indexAtCoords(x, y, indice)) {return false;}

    return addValueAtIndex(indice, value);
}

template< typename DataT>
QList<DataT> CT_Image2D<DataT>::neighboursValues(const size_t col, const size_t lin, const size_t distance, const bool keepNAs, const CenterMode centermode) const
{
    QList<DataT> liste;

    if (distance < 1) {return liste;}
    if (col >= _dimCol) {return liste;}
    if (lin >= _dimLin) {return liste;}

    size_t firstcol = col-distance;
    size_t lastcol = col+distance;
    size_t firstlin = lin-distance;
    size_t lastlin = lin+distance;

    if (firstcol >= _dimCol) {firstcol = 0;}
    if (lastcol >= _dimCol) {lastcol = _dimCol - 1;}
    if (firstlin >= _dimLin) {firstlin = 0;}
    if (lastlin >= _dimLin) {lastlin = _dimLin - 1;}

    for (size_t xx = firstcol ; xx <= lastcol ; xx++)
    {
        for (size_t yy = firstlin ; yy <= lastlin ; yy++)
        {
            DataT value = this->value(xx, yy);
            if ((xx == col) && (yy == lin)) {
                if (centermode == CM_KeepCenter) {
                    if ((value != NA()) || keepNAs) {liste.append(value);}
                } else if (centermode == CM_NAasCenter) {
                    liste.append(NA());
                }
            } else {
                if ((value != NA()) || keepNAs) {liste.append(value);}
            }
        }
    }

    return liste;
}

#endif // CT_IMAGE2D_HPP
