#ifndef CT_GRID4D_SPARSE_HPP
#define CT_GRID4D_SPARSE_HPP

#ifdef USE_OPENCV


#include "ct_itemdrawable/ct_grid4d_sparse.h"

#include <math.h>
#include <typeinfo>

#include "ct_math/ct_math.h"

template< typename DataT>
CT_Grid4D_Sparse<DataT>::CT_Grid4D_Sparse() : CT_Grid4D<DataT>()
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>::CT_Grid4D_Sparse(const CT_OutAbstractSingularItemModel *model,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        size_t dimw,
                                        size_t dimx,
                                        size_t dimy,
                                        size_t dimz,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(model,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            dimw,
                                                                            dimx,
                                                                            dimy,
                                                                            dimz,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>::CT_Grid4D_Sparse(const QString &modelName,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        size_t dimw,
                                        size_t dimx,
                                        size_t dimy,
                                        size_t dimz,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(modelName,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            dimw,
                                                                            dimx,
                                                                            dimy,
                                                                            dimz,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>::CT_Grid4D_Sparse(const CT_OutAbstractSingularItemModel *model,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        double wmax,
                                        double xmax,
                                        double ymax,
                                        double zmax,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(model,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            wmax,
                                                                            xmax,
                                                                            ymax,
                                                                            zmax,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>::CT_Grid4D_Sparse(const QString& modelName,
                                        const CT_AbstractResult *result,
                                        double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        double wmax,
                                        double xmax,
                                        double ymax,
                                        double zmax,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : CT_Grid4D<DataT>(modelName,
                                                                            result,
                                                                            wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            wmax,
                                                                            xmax,
                                                                            ymax,
                                                                            zmax,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>* CT_Grid4D_Sparse<DataT>::createGrid4DFromWXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Sparse<DataT>(model, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>* CT_Grid4D_Sparse<DataT>::createGrid4DFromWXYZCoords(const QString &modelName,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Sparse<DataT>(modelName, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Sparse<DataT>::~CT_Grid4D_Sparse()
{
    _data.release();
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D_Sparse<DataT>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D_Sparse<DataT>* cpy = new CT_Grid4D_Sparse<DataT>((const CT_OutAbstractSingularItemModel *)model, result, this->_bot.w(), this->_bot.x(), this->_bot.y(), this->_bot.z(), this->_dimw, this->_dimx, this->_dimy, this->_dimz, this->_resw, this->_resx, this->_resy, this->_resz, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());

    size_t ncells = this->nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( this->getAlternativeDrawManager() );

    return cpy;
}

template< typename DataT>
CT_AbstractItemDrawable* CT_Grid4D_Sparse<DataT>::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Grid4D_Sparse<DataT>* cpy = new CT_Grid4D_Sparse<DataT>(modelName, result, this->_bot.w(), this->_bot.x(), this->_bot.y(), this->_bot.z(), this->_dimw, this->_dimx, this->_dimy, this->_dimz, this->_resw, this->_resx, this->_resy, this->_resz, this->_NAdata, this->_NAdata);
    cpy->setId(this->id());

    size_t ncells = this->nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        cpy->setValueAtIndex(i, valueAtIndex(i));
    }

    if ( ncells > 0 )
    {
        cpy->computeMinMax();
    }

    cpy->setAlternativeDrawManager( this->getAlternativeDrawManager() );

    return cpy;
}


template< typename DataT>
void CT_Grid4D_Sparse<DataT>::computeMinMax()
{
    this->_dataMin = this->NA();
    this->_dataMax = this->NA();

    bool first = true;
    cv::SparseMatConstIterator it = this->_data.begin(), it_end = this->_data.end();
    for( ; it != it_end; ++it )
    {
       DataT val = it.value<DataT>();
       if (first)
       {
           this->_dataMin = val;
           this->_dataMax = val;
           first = false;
       } else {
           if (val < this->_dataMin) {this->_dataMin = val;}
           if (val > this->_dataMax) {this->_dataMax = val;}
       }
    }
}

template< typename DataT>
QString CT_Grid4D_Sparse<DataT>::getType() const
{
    return staticGetType();
}

template< typename DataT>
QString CT_Grid4D_Sparse<DataT>::staticGetType()
{
    QString type = CT_AbstractGrid4D::staticGetType() + "/CT_Grid4D_Sparse<" + CT_TypeInfo::name<DataT>() + ">";
    CT_AbstractItemDrawable::addNameTypeCorresp(type, staticName());
    return type;
}

template< typename DataT>
QString CT_Grid4D_Sparse<DataT>::name() const
{
    return staticName();
}

template< typename DataT>
QString CT_Grid4D_Sparse<DataT>::staticName()
{
    return CT_Grid4D_Sparse<DataT>::tr("4D grid<%1>, sparse").arg(CT_TypeInfo::name<DataT>());
}



#endif

#endif // CT_GRID4D_SPARSE_HPP
