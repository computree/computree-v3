#ifndef CT_IMAGE2DTOOLS_H
#define CT_IMAGE2DTOOLS_H

#ifdef USE_OPENCV

#include "pluginShared_global.h"
#include "ct_itemdrawable/ct_image2d.h"

class PLUGINSHAREDSHARED_EXPORT CT_Image2DNaturalNeighboursInterpolator
{
public:

    CT_Image2DNaturalNeighboursInterpolator(CT_Image2D<float>* raster, CT_Image2D<float>* rasterI, int ncells);

    typedef void result_type;
    void operator()(const size_t &index)
    {
        interpolateForOneCell(index);
    }

    void interpolateForOneCell(const size_t &index);

private:
    CT_Image2D<float>* _raster;
    CT_Image2D<float>* _rasterInterpol;
    int     _nCells;


};

#endif

#endif // CT_IMAGE2DTOOLS_H
