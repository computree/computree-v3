#ifndef CT_GRID4D_HPP
#define CT_GRID4D_HPP

#include "ct_itemdrawable/tools/drawmanager/ct_standardgrid4ddrawmanager.h"
#include "ct_itemdrawable/ct_grid4d.h"

#include <math.h>
#include <typeinfo>

#include "ct_math/ct_math.h"
#include "qdebug.h"

template< typename DataT> CT_DEFAULT_IA_INIT(CT_Grid4D<DataT>)

template< typename DataT>
const CT_StandardGrid4DDrawManager<DataT> CT_Grid4D<DataT>::ABSGRID4D_DRAW_MANAGER;

template< typename DataT>
CT_Grid4D<DataT>::CT_Grid4D() : CT_AbstractGrid4D()
{
    // _minCoord and _maxCoord comes fromabstractsingularitemdrawable
    // Mainly used for visualisation purpose (display a bbox and center camera)
    _minCoordinates.setX(0);
    _minCoordinates.setY(0);
    _minCoordinates.setZ(0);
    _maxCoordinates.setX(0);
    _maxCoordinates.setY(0);
    _maxCoordinates.setZ(0);

    // We also set the 4D bounding box from the class
    _bot.setW(0);
    _bot.setX(0);
    _bot.setY(0);
    _bot.setZ(0);
    _top.setW(0);
    _top.setX(0);
    _top.setY(0);
    _top.setZ(0);

    _resw = 0;
    _resx = 0;
    _resy = 0;
    _resz = 0;
    _dimw = 0;
    _dimx = 0;
    _dimy = 0;
    _dimz = 0;
    _NAdata = -1;
    _dataMax = -1;
    _dataMin = -1;

    setBaseDrawManager(&ABSGRID4D_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid4D<DataT>::CT_Grid4D(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result,
                            float wmin,
                            float xmin,
                            float ymin,
                            float zmin,
                            size_t dimw,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            float resw,
                            float resx,
                            float resy,
                            float resz,
                            DataT na,
                            DataT initValue) : CT_AbstractGrid4D(model, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);
    _maxCoordinates.setX(minX() + resx * dimx);
    _maxCoordinates.setY(minY() + resy * dimy);
    _maxCoordinates.setZ(minZ() + resz * dimz);

    _bot.setW( wmin );
    _bot.setX( xmin );
    _bot.setY( ymin );
    _bot.setZ( zmin );

    _top.setW( wmin + resw * dimw );
    _top.setX( xmin + resx * dimx );
    _top.setY( ymin + resy * dimy );
    _top.setZ( zmin + resz * dimz );

    _dimw = dimw;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _resw = resw;
    _resx = resx;
    _resy = resy;
    _resz = resz;

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());
    initGridWithValue(initValue);

    setBaseDrawManager(&ABSGRID4D_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid4D<DataT>::CT_Grid4D(const QString &modelName,
                            const CT_AbstractResult *result,
                            float wmin,
                            float xmin,
                            float ymin,
                            float zmin,
                            size_t dimw,
                            size_t dimx,
                            size_t dimy,
                            size_t dimz,
                            float resw,
                            float resx,
                            float resy,
                            float resz,
                            DataT na,
                            DataT initValue) : CT_AbstractGrid4D(modelName, result)
{
    _minCoordinates.setX(xmin);
    _minCoordinates.setY(ymin);
    _minCoordinates.setZ(zmin);
    _maxCoordinates.setX(minX() + resx * dimx);
    _maxCoordinates.setY(minY() + resy * dimy);
    _maxCoordinates.setZ(minZ() + resz * dimz);

    _bot.setW( wmin );
    _bot.setX( xmin );
    _bot.setY( ymin );
    _bot.setZ( zmin );

    _top.setW( wmin + resw * dimw );
    _top.setX( xmin + resx * dimx );
    _top.setY( ymin + resy * dimy );
    _top.setZ( zmin + resz * dimz );

    _dimw = dimw;
    _dimx = dimx;
    _dimy = dimy;
    _dimz = dimz;

    _resw = resw;
    _resx = resx;
    _resy = resy;
    _resz = resz;

    _NAdata = na;

    setCenterX (minX() + (maxX() - minX())/2.0);
    setCenterY (minY() + (maxY() - minY())/2.0);
    setCenterZ (minZ() + (maxZ() - minZ())/2.0);

    _data.resize(nCells());
    initGridWithValue(initValue);

    setBaseDrawManager(&ABSGRID4D_DRAW_MANAGER);
}


template< typename DataT>
CT_Grid4D<DataT>* CT_Grid4D<DataT>::createGrid4DFromWXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                    const CT_AbstractResult *result,
                                                    float wmin,
                                                    float xmin,
                                                    float ymin,
                                                    float zmin,
                                                    float wmax,
                                                    float xmax,
                                                    float ymax,
                                                    float zmax,
                                                    float resw,
                                                    float resx,
                                                    float resy,
                                                    float resz,
                                                    DataT na,
                                                    DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    CT_Grid4D<DataT>* grid = new CT_Grid4D((const CT_OutAbstractSingularItemModel *)model, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= grid->maxW())
    {
        grid->_dimw++;
        grid->_top.setW( grid->_top.w() + resw);
    }

    while (xmax >= grid->maxX())
    {
        grid->_dimx++;
        grid->_maxCoordinates.setX(grid->maxX() + resx);
        grid->_top.setX( grid->_top.x() + resx);
    }

    while (ymax >= grid->maxY())
    {
        grid->_dimy++;
        grid->_maxCoordinates.setY(grid->maxY() + resy);
        grid->_top.setY( grid->_top.y() + resy);
    }

    while (zmax >= grid->maxZ())
    {
        grid->_dimz++;
        grid->_maxCoordinates.setZ(grid->maxZ() + resz);
        grid->_top.setZ( grid->_top.z() + resz);
    }

    return grid;
}

template< typename DataT>
CT_Grid4D<DataT>* CT_Grid4D<DataT>::createGrid4DFromWXYZCoords(const QString &modelName,
                                                    const CT_AbstractResult *result,
                                                    float wmin,
                                                    float xmin,
                                                    float ymin,
                                                    float zmin,
                                                    float wmax,
                                                    float xmax,
                                                    float ymax,
                                                    float zmax,
                                                    float resw,
                                                    float resx,
                                                    float resy,
                                                    float resz,
                                                    DataT na,
                                                    DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    CT_Grid4D<DataT>* grid = new CT_Grid4D((const CT_OutAbstractSingularItemModel *)model, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= grid->maxW())
    {
        grid->_dimw++;
        grid->_top.setW( grid->_top.w() + resw);
    }

    while (xmax >= grid->maxX())
    {
        grid->_dimx++;
        grid->_maxCoordinates.setX(grid->maxX() + resx);
        grid->_top.setX( grid->_top.x() + resx);
    }

    while (ymax >= grid->maxY())
    {
        grid->_dimy++;
        grid->_maxCoordinates.setY(grid->maxY() + resy);
        grid->_top.setY( grid->_top.y() + resy);
    }

    while (zmax >= grid->maxZ())
    {
        grid->_dimz++;
        grid->_maxCoordinates.setZ(grid->maxZ() + resz);
        grid->_top.setZ( grid->_top.z() + resz);
    }

    return grid;
}

template< typename DataT>
CT_Grid4D<DataT>::~CT_Grid4D()
{
    _data.clear();
}

template< typename DataT>
void CT_Grid4D<DataT>::initGridWithValue(const DataT val)
{
    for (size_t  i = 0 ; i < nCells() ; i++)
    {
        _data[i] = val;
    }
    _dataMin = val;
    _dataMax = val;
}

template< typename DataT>
float CT_Grid4D<DataT>::ratioValueAtIndex(const size_t index) const
{
    if (_dataMax <= _dataMin)
    {
        return 1;
    }

    DataT value = valueAtIndex(index);

    if ( value == NA() )
    {
        return -1;
    }

    return (float) (((double)(value - _dataMin))/((double)(_dataMax - _dataMin)));
}

template< typename DataT>
DataT CT_Grid4D<DataT>::value(const size_t levw, const size_t levx, const size_t levy, const size_t levz) const
{
    size_t i;
    if( index( levw, levx, levy, levz, i) )
    {
        return valueAtIndex(i);
    }
    return NA();
}

template< typename DataT>
DataT CT_Grid4D<DataT>::valueAtWXYZ(const float w, const float x, const float y, const float z) const
{
    size_t i;
    if ( indexAtWXYZ(w, x, y, z, i) )
    {
        return valueAtIndex(i);
    }
    return false;
}

template< typename DataT>
bool CT_Grid4D<DataT>::addValueAtIndex(const size_t index, const DataT value)
{
    if ( index >= nCells() )
    {
        return false;
    }

    DataT currentValue = _data[index];

    if (currentValue == NA())
    {
        return setValueAtIndex(index, value);
    }

    else if ( value != NA() )
    {
        return setValueAtIndex(index, value + currentValue);
    }

    return false;
}

template< typename DataT>
bool CT_Grid4D<DataT>::addValueAtWXYZ(const float w, const float x, const float y, const float z, const DataT value)
{
    size_t i;

    if( indexAtWXYZ(x, y, z, i) )
    {
        return addValueAtIndex(i, value);
    }

    return false;
}

template< typename DataT>
bool CT_Grid4D<DataT>::neighboursValues(const size_t levw,
                                        const size_t levx,
                                        const size_t levy,
                                        const size_t levz,
                                        const size_t distance,
                                        QList<DataT>& outNeighboursValues,
                                        const bool keepNAs,
                                        const CenterMode centermode) const
{
    outNeighboursValues.clear();

    if (distance < 1) {return false;}
    if (levw >= _dimw || levw < 0 ) {return false;}
    if (levx >= _dimx || levx < 0 ) {return false;}
    if (levy >= _dimy || levy < 0 ) {return false;}
    if (levz >= _dimz || levz < 0 ) {return false;}

    size_t firstLevw = levw-distance;
    size_t lastLevw = levw+distance;
    size_t firstLevx = levx-distance;
    size_t lastLevx = levx+distance;
    size_t firstLevy = levy-distance;
    size_t lastLevy = levy+distance;
    size_t firstLevz = levz-distance;
    size_t lastLevz = levz+distance;

    if (firstLevw >= _dimw) {firstLevw = 0;}
    if (lastLevw >= _dimw) {lastLevw = _dimw - 1;}
    if (firstLevx >= _dimx) {firstLevx = 0;}
    if (lastLevx >= _dimx) {lastLevx = _dimx - 1;}
    if (firstLevy >= _dimy) {firstLevy = 0;}
    if (lastLevy >= _dimy) {lastLevy = _dimy - 1;}
    if (firstLevz >= _dimz) {firstLevz = 0;}
    if (lastLevz >= _dimz) {lastLevz = _dimz - 1;}


    for (size_t ww = firstLevw ; ww <= lastLevw ; ww++)
    {
        for (size_t xx = firstLevx ; xx <= lastLevx ; xx++)
        {
            for (size_t yy = firstLevy ; yy <= lastLevy ; yy++)
            {
                for (size_t zz = firstLevz ; zz <= lastLevz ; zz++)
                {
                    DataT val = value(ww, xx, yy, zz);

                    if ( (ww == levw) && (xx == levx) && (yy == levy) && (zz == levz) )
                    {
                        if( centermode == CM_KeepCenter )
                        {
                            if ( (val != NA()) || keepNAs )
                            {
                                outNeighboursValues.append(val);
                            }
                        }

                        else if ( centermode == CM_NAasCenter )
                        {
                            outNeighboursValues.append(NA());
                        }
                    }

                    else
                    {
                        if ( (val != NA()) || keepNAs )
                        {
                            outNeighboursValues.append(val);
                        }
                    }
                }
            }
        }
    }

    return true;
}

template< typename DataT>
DataT CT_Grid4D<DataT>::dataFromArray(const size_t &w, const size_t &x, const size_t &y, const size_t &z) const
{
    size_t i;
    if ( index(w,x, y, z, i) )
    {
        return valueAtIndex(i);
    }
    return NA();
}

template< typename DataT>
DataT CT_Grid4D<DataT>::dataFromArray(const size_t &index) const
{
    if ( index >= nCells() )
    {
        return NA();
    }
    return _data[index];
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D<DataT>* cpy = new CT_Grid4D<DataT>((const CT_OutAbstractSingularItemModel *)model, result, _bot.w(), _bot.x(), _bot.y(), _bot.z(), _dimw, _dimx, _dimy, _dimz, _resw, _resx, _resy, _resz, _NAdata, _NAdata);
    cpy->setId(id());

    size_t ncells = nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( getAlternativeDrawManager() );

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D<DataT>* cpy = new CT_Grid4D<DataT>(modelName, result, _bot.w(), _bot.x(), _bot.y(), _bot.z(), _dimw, _dimx, _dimy, _dimz, _resw, _resx, _resy, _resz, _NAdata, _NAdata);
    cpy->setId(id());

    size_t ncells = nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( getAlternativeDrawManager() );

    return cpy;
}

template< typename DataT>
void CT_Grid4D<DataT>::computeMinMax()
{
    size_t ncells = nCells();
    if (ncells > 0)
    {
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

#endif // CT_GRID4D_HPP
