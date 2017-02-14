/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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

#include "ct_colorcomposite.h"

#ifdef USE_OPENCV

#include <limits>

const CT_StandardColorCompositeDrawManager CT_ColorComposite::COLOR_COMPOSITE_DRAW_MANAGER;

CT_ColorComposite::CT_ColorComposite() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _red = NULL;
    _green = NULL;
    _blue = NULL;
    _zValue = NULL;

    _redSource = NULL;
    _greenSource = NULL;
    _blueSource = NULL;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&COLOR_COMPOSITE_DRAW_MANAGER);
}

CT_ColorComposite::CT_ColorComposite(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result,
                                     CT_AbstractImage2D* red, CT_AbstractImage2D* green, CT_AbstractImage2D* blue, CT_Image2D<float>* zvalue) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _red = NULL;
    _green = NULL;
    _blue = NULL;
    _zValue = NULL;
    _redSource = red;
    _greenSource = green;
    _blueSource = blue;

    if (computeBands(red, green, blue, zvalue))
    {
        setCenterX((red->maxX() + red->minX() / 2.0));
        setCenterY((red->maxY() + red->minY() / 2.0));
        setCenterZ(0);

        _minCoordinates(0) = red->minX();
        _minCoordinates(1) = red->minY();
        _minCoordinates(2) = 0;

        _maxCoordinates(0) = red->maxX();
        _maxCoordinates(1) = red->maxY();
        _maxCoordinates(2) = 0;

    } else {
        setCenterX( 0 );
        setCenterY( 0 );
        setCenterZ( 0 );
    }

    setBaseDrawManager(&COLOR_COMPOSITE_DRAW_MANAGER);
}


CT_ColorComposite::CT_ColorComposite(const QString &modelName, const CT_AbstractResult *result,
                                     CT_AbstractImage2D* red, CT_AbstractImage2D* green, CT_AbstractImage2D* blue, CT_Image2D<float>* zvalue) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _red = NULL;
    _green = NULL;
    _blue = NULL;
    _zValue = NULL;
    _redSource = red;
    _greenSource = green;
    _blueSource = blue;

    if (computeBands(red, green, blue, zvalue))
    {
        setCenterX((red->maxX() + red->minX() / 2.0));
        setCenterY((red->maxY() + red->minY() / 2.0));
        setCenterZ(0);

        _minCoordinates(0) = red->minX();
        _minCoordinates(1) = red->minY();
        _minCoordinates(2) = 0;

        _maxCoordinates(0) = red->maxX();
        _maxCoordinates(1) = red->maxY();
        _maxCoordinates(2) = 0;

    } else {
        setCenterX( 0 );
        setCenterY( 0 );
        setCenterZ( 0 );
    }

    setBaseDrawManager(&COLOR_COMPOSITE_DRAW_MANAGER);
}

bool CT_ColorComposite::computeBands(CT_AbstractImage2D* red, CT_AbstractImage2D* green, CT_AbstractImage2D* blue, CT_Image2D<float>* zvalue)
{
    if (red != NULL && green != NULL && blue != NULL &&
            red->minX() == green->minX() && red->minX() == green->minX() &&
            red->minY() == green->minY() && red->minY() == green->minY() &&
            red->colDim() == green->colDim() && red->colDim() == green->colDim() &&
            red->linDim() == green->linDim() && red->linDim() == green->linDim() &&
            red->resolution() == green->resolution() && red->resolution() == green->resolution())
    {
        _zValue = zvalue;

        _red = new CT_Image2D<quint8>(NULL, NULL, red->minX(), red->minY(), red->colDim(), red->linDim(), red->resolution(), 0, 0, 0);
        _green = new CT_Image2D<quint8>(NULL, NULL, red->minX(), red->minY(), red->colDim(), red->linDim(), red->resolution(), 0, 0, 0);
        _blue = new CT_Image2D<quint8>(NULL, NULL, red->minX(), red->minY(), red->colDim(), red->linDim(), red->resolution(), 0, 0, 0);

        double minRed = red->minValueAsDouble();
        double minGreen = green->minValueAsDouble();
        double minBlue = blue->minValueAsDouble();
        double amplitudeRed = red->maxValueAsDouble() - minRed;
        double amplitudeGreen = green->maxValueAsDouble() - minGreen;
        double amplitudeBlue = blue->maxValueAsDouble() - minBlue;

        for (size_t xx = 0 ; xx < red->colDim() ; xx++)
        {
            for (size_t yy = 0 ; yy < red->linDim() ; yy++)
            {
                size_t index;
                if (red->index(xx, yy, index))
                {
                    double redV = red->valueAtIndexAsDouble(index);
                    double greenV = green->valueAtIndexAsDouble(index);
                    double blueV = blue->valueAtIndexAsDouble(index);

                    _red->setValue(xx, yy, 255*(redV / amplitudeRed));
                    _green->setValue(xx, yy, 255*(greenV / amplitudeGreen));
                    _blue->setValue(xx, yy, 255*(blueV / amplitudeBlue));
                }
            }
        }
        _red->setMinMax(0, 255);
        _green->setMinMax(0, 255);
        _blue->setMinMax(0, 255);

        return true;
    }
    return false;
}

CT_ColorComposite::~CT_ColorComposite()
{
    if (_red != NULL) {delete _red;}
    if (_green != NULL) {delete _green;}
    if (_blue != NULL) {delete _blue;}
    _zValue = NULL;
    _redSource = NULL;
    _greenSource = NULL;
    _blueSource = NULL;
}

CT_AbstractItemDrawable* CT_ColorComposite::copy(const CT_OutAbstractItemModel *model,
                                                 const CT_AbstractResult *result,
                                                 CT_ResultCopyModeList copyModeList)
{
    CT_ColorComposite *item = new CT_ColorComposite((const CT_OutAbstractSingularItemModel *)model, result, _redSource, _greenSource, _blueSource, _zValue);
    item->setId(id());

    item->setAlternativeDrawManager(getAlternativeDrawManager());

    return item;
}

#endif
