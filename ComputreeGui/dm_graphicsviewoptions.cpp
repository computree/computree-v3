/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "dm_graphicsviewoptions.h"
#include "cdm_configfile.h"

DM_GraphicsViewOptions::DM_GraphicsViewOptions()
{
    _backgroundColor = QColor(48, 48, 48);
    _selectedColor = Qt::red;
    _pointSize = 1.0;
    _drawAxis = false;
    _useColor = true;
    _drawFastest = Normal;
    _useTransparency = false;
    _useLight = false;
    _fastDrawTime = 1500;
    _cameraInfoDisplayed = DM_GraphicsViewOptions::CameraNone;
    _cameraInfoPosition = DM_GraphicsViewOptions::InfoInLowerRightCorner;
    _cameraType = CameraInterface::PERSPECTIVE;
    m_octreeNumberOfCells = 32;
    m_showOctree = false;
}

DM_GraphicsViewOptions::~DM_GraphicsViewOptions()
{
}

void DM_GraphicsViewOptions::drawAxis(bool draw)
{
    _drawAxis = draw;
}

void DM_GraphicsViewOptions::useColor(bool enable)
{
    _useColor = enable;
}

void DM_GraphicsViewOptions::useTransparency(bool use)
{
    _useTransparency = use;
}

void DM_GraphicsViewOptions::useLight(bool use)
{
    _useLight = use;
}

void DM_GraphicsViewOptions::drawFastest(DM_GraphicsViewOptions::DrawFastestMode mode)
{
    _drawFastest = mode;
}

void DM_GraphicsViewOptions::setSelectedColor(QColor &color)
{
    _selectedColor = color;
}

void DM_GraphicsViewOptions::setBackgroudColor(QColor &color)
{
    _backgroundColor = color;
}

void DM_GraphicsViewOptions::setPointSize(float size)
{
    _pointSize = size;
}

void DM_GraphicsViewOptions::setFastDrawTime(int time)
{
    _fastDrawTime = time;
}

void DM_GraphicsViewOptions::setCameraInformationDisplayed(DM_GraphicsViewOptions::CameraInfoDisplayed info)
{
    _cameraInfoDisplayed = info;
}

void DM_GraphicsViewOptions::setCameraInformationPosition(DM_GraphicsViewOptions::CameraInfoPosition pos)
{
    _cameraInfoPosition = pos;
}

void DM_GraphicsViewOptions::setCameraType(CameraInterface::CameraType type)
{
    _cameraType = type;
}

void DM_GraphicsViewOptions::setShowOctree(bool val)
{
    m_showOctree = val;
}

void DM_GraphicsViewOptions::setOctreeNumberOfCells(int n)
{
    // must be a power of two
    if(((n - 1) & n) != 0)
        return;

    m_octreeNumberOfCells = n;
}

void DM_GraphicsViewOptions::updateFromOtherOptions(const DM_GraphicsViewOptions &options)
{
    bool emitChanged = false;

    if(_backgroundColor != options._backgroundColor)
    {
        _backgroundColor = options._backgroundColor;
        emitChanged = true;
    }

    if(_selectedColor != options._selectedColor)
    {
        _selectedColor = options._selectedColor;
        emitChanged = true;
    }

    if(_pointSize != options._pointSize)
    {
        _pointSize = options._pointSize;
        emitChanged = true;
    }

    if(_drawAxis != options._drawAxis)
    {
        _drawAxis = options._drawAxis;
        emitChanged = true;
    }

    if(_useColor != options._useColor)
    {
        _useColor = options._useColor;
        emitChanged = true;
    }

    if(_drawFastest != options._drawFastest)
    {
        _drawFastest = options._drawFastest;
        emitChanged = true;
    }

    if(_useTransparency != options._useTransparency)
    {
        _useTransparency = options._useTransparency;
        emitChanged = true;
    }

    if(_useLight != options._useLight)
    {
        _useLight = options._useLight;
        emitChanged = true;
    }

    if(_fastDrawTime != options._fastDrawTime)
    {
        _fastDrawTime = options._fastDrawTime;
        emitChanged = true;
    }

    if(_cameraInfoDisplayed != options._cameraInfoDisplayed)
    {
        _cameraInfoDisplayed = options._cameraInfoDisplayed;
        emitChanged = true;
    }

    if(_cameraInfoPosition != options._cameraInfoPosition)
    {
        _cameraInfoPosition = options._cameraInfoPosition;
        emitChanged = true;
    }

    if(_cameraType != options._cameraType)
    {
        _cameraType = options._cameraType;
        emitChanged = true;
    }

    if(m_showOctree != options.m_showOctree)
    {
        m_showOctree = options.m_showOctree;
        emitChanged = true;
    }

    if(m_octreeNumberOfCells != options.m_octreeNumberOfCells)
    {
        m_octreeNumberOfCells = options.m_octreeNumberOfCells;
        emitChanged = true;
    }

    if(emitChanged)
        emit optionsChanged();
}

bool DM_GraphicsViewOptions::load()
{
    CONFIG_FILE->beginGroup("GraphicsOptions");

    _backgroundColor = CONFIG_FILE->colorValue("backgroundColor", _backgroundColor);
    _selectedColor = CONFIG_FILE->colorValue("selectedColor", _selectedColor);
    _pointSize = CONFIG_FILE->value("pointSize", (double)_pointSize).toDouble();
    _drawAxis = CONFIG_FILE->value("drawAxis", _drawAxis).toBool();
    _useColor = CONFIG_FILE->value("useColor", _useColor).toBool();
    _drawFastest = (DrawFastestMode)CONFIG_FILE->value("drawFastest", (int)_drawFastest).toInt();
    _useTransparency = CONFIG_FILE->value("useTransparency", _useTransparency).toBool();
    _useLight = CONFIG_FILE->value("useLight", _useLight).toBool();
    _fastDrawTime = CONFIG_FILE->value("fastDrawTime", _fastDrawTime).toInt();
    _cameraInfoDisplayed = (CameraInfoDisplayed)CONFIG_FILE->value("cameraInfoDisplayed", _cameraInfoDisplayed.operator int()).toInt();
    _cameraInfoPosition = (CameraInfoPosition)CONFIG_FILE->value("cameraInfoPosition", (int)_cameraInfoPosition).toInt();
    _cameraType = (CameraInterface::CameraType)CONFIG_FILE->value("cameraType", (int)_cameraType).toInt();
    m_showOctree = CONFIG_FILE->value("showOctree", m_showOctree).toBool();
    setOctreeNumberOfCells(CONFIG_FILE->value("octreeNumberOfCells", m_octreeNumberOfCells).toInt());

    CONFIG_FILE->endGroup();

    return true;
}

bool DM_GraphicsViewOptions::save()
{
    CONFIG_FILE->beginGroup("GraphicsOptions");

    CONFIG_FILE->setColorValue("backgroundColor", _backgroundColor);
    CONFIG_FILE->setColorValue("selectedColor", _selectedColor);
    CONFIG_FILE->setValue("pointSize", (double)_pointSize);
    CONFIG_FILE->setValue("drawAxis", _drawAxis);
    CONFIG_FILE->setValue("useColor", _useColor);
    CONFIG_FILE->setValue("drawFastest", (int)_drawFastest);
    CONFIG_FILE->setValue("useTransparency", _useTransparency);
    CONFIG_FILE->setValue("useLight", _useLight);
    CONFIG_FILE->setValue("fastDrawTime", _fastDrawTime);
    CONFIG_FILE->setValue("cameraInfoDisplayed", _cameraInfoDisplayed.operator int());
    CONFIG_FILE->setValue("cameraInfoPosition", (int)_cameraInfoPosition);
    CONFIG_FILE->setValue("cameraType", (int)_cameraType);
    CONFIG_FILE->setValue("showOctree", _cameraType);
    CONFIG_FILE->setValue("octreeNumberOfCells", m_octreeNumberOfCells);

    CONFIG_FILE->endGroup();

    return true;
}
