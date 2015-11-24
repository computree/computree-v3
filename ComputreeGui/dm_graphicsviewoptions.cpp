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

#include "dm_domutils.h"

DM_GraphicsViewOptions::DM_GraphicsViewOptions()
{
    _backgroundColor = QColor(48, 48, 48);
    _selectedColor = Qt::red;
    _pointSize = 1.0;
    _drawAxis = true;
    m_drawGrid = false;
    _useColor = true;
    _useTransparency = false;
    _useLight = false;
    _cameraInfoDisplayed = DM_GraphicsViewOptions::CameraNone;
    _cameraInfoPosition = DM_GraphicsViewOptions::InfoInLowerRightCorner;
    _cameraType = CameraInterface::PERSPECTIVE;
    m_normalColor = Qt::yellow;
    m_normalLength = 0.05;
}

DM_GraphicsViewOptions::~DM_GraphicsViewOptions()
{
}

void DM_GraphicsViewOptions::drawAxis(bool draw)
{
    _drawAxis = draw;
}

void DM_GraphicsViewOptions::setDrawGrid(bool draw)
{
    m_drawGrid = draw;
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

void DM_GraphicsViewOptions::setNormalColor(const QColor &color)
{
    m_normalColor = color;
}

void DM_GraphicsViewOptions::setNormalLength(float l)
{
    m_normalLength = l;
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

        emit selectionColorChanged(_selectedColor);

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

    if(m_drawGrid != options.m_drawGrid)
    {
        m_drawGrid = options.m_drawGrid;
        emitChanged = true;
    }

    if(_useColor != options._useColor)
    {
        _useColor = options._useColor;
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

    if(m_normalColor != options.m_normalColor)
    {
        m_normalColor = options.m_normalColor;
        emit normalColorChanged(m_normalColor);

        emitChanged = true;
    }

    if(m_normalLength != options.m_normalLength)
    {
        m_normalLength = options.m_normalLength;
        emit normalLengthChanged(m_normalLength);

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
    m_drawGrid = CONFIG_FILE->value("drawGrid", drawGrid()).toBool();
    _useColor = CONFIG_FILE->value("useColor", _useColor).toBool();
    _useTransparency = CONFIG_FILE->value("useTransparency", _useTransparency).toBool();
    _useLight = CONFIG_FILE->value("useLight", _useLight).toBool();
    _cameraInfoDisplayed = (CameraInfoDisplayed)CONFIG_FILE->value("cameraInfoDisplayed", _cameraInfoDisplayed.operator int()).toInt();
    _cameraInfoPosition = (CameraInfoPosition)CONFIG_FILE->value("cameraInfoPosition", (int)_cameraInfoPosition).toInt();
    _cameraType = (CameraInterface::CameraType)CONFIG_FILE->value("cameraType", (int)_cameraType).toInt();
    setNormalColor(CONFIG_FILE->colorValue("normalColor", m_normalColor));
    setNormalLength(CONFIG_FILE->value("normalLength", m_normalLength).toFloat());

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
    CONFIG_FILE->setValue("drawGrid", m_drawGrid);
    CONFIG_FILE->setValue("useColor", _useColor);
    CONFIG_FILE->setValue("useTransparency", _useTransparency);
    CONFIG_FILE->setValue("useLight", _useLight);
    CONFIG_FILE->setValue("cameraInfoDisplayed", _cameraInfoDisplayed.operator int());
    CONFIG_FILE->setValue("cameraInfoPosition", (int)_cameraInfoPosition);
    CONFIG_FILE->setValue("cameraType", (int)_cameraType);
    CONFIG_FILE->setColorValue("normalColor", m_normalColor);
    CONFIG_FILE->setValue("normalLength", m_normalLength);

    CONFIG_FILE->endGroup();

    return true;
}

bool DM_GraphicsViewOptions::loadFromXml(const QDomElement &el)
{
    QDomNodeList l = el.elementsByTagName("backgroundColor");

    if(!l.isEmpty())
        _backgroundColor = DM_DomUtils::QColorFromDom(l.at(0).toElement());

    l = el.elementsByTagName("selectedColor");

    if(!l.isEmpty())
        _selectedColor = DM_DomUtils::QColorFromDom(l.at(0).toElement());

    _pointSize = DM_DomUtils::qrealFromDom(el, "pointSize", _pointSize);
    _drawAxis = DM_DomUtils::boolFromDom(el, "drawAxis", _drawAxis);
    m_drawGrid = DM_DomUtils::boolFromDom(el, "drawGrid", m_drawGrid);
    _useColor = DM_DomUtils::boolFromDom(el, "useColor", _useColor);
    _useTransparency = DM_DomUtils::boolFromDom(el, "useTransparency", _useTransparency);
    _useLight = DM_DomUtils::boolFromDom(el, "useLight", _useLight);
    _cameraInfoDisplayed = (CameraInfoDisplayed)DM_DomUtils::intFromDom(el, "cameraInfoDisplayed", (int)_cameraInfoDisplayed);
    _cameraInfoPosition = (CameraInfoPosition) DM_DomUtils::intFromDom(el, "cameraInfoPosition", (int)_cameraInfoPosition);
    _cameraType = (CameraInterface::CameraType) DM_DomUtils::intFromDom(el, "cameraType", (int)_cameraType);

    l = el.elementsByTagName("normalColor");

    if(!l.isEmpty())
        setNormalColor(DM_DomUtils::QColorFromDom(l.at(0).toElement()));

    setNormalLength(DM_DomUtils::qrealFromDom(el, "normalLength", m_normalLength));

    return true;
}

bool DM_GraphicsViewOptions::saveToXml(QDomElement &main, QDomDocument &doc) const
{
    main.appendChild(DM_DomUtils::QColorDomElement(_backgroundColor, "backgroundColor", doc));
    main.appendChild(DM_DomUtils::QColorDomElement(_selectedColor, "selectedColor", doc));
    main.setAttribute("pointSize", _pointSize);
    DM_DomUtils::setBoolAttribute(main, "drawAxis", _drawAxis);
    DM_DomUtils::setBoolAttribute(main, "drawGrid", m_drawGrid);
    DM_DomUtils::setBoolAttribute(main, "useColor", _useColor);
    DM_DomUtils::setBoolAttribute(main, "useTransparency", _useTransparency);
    DM_DomUtils::setBoolAttribute(main, "useLight", _useLight);
    main.setAttribute("cameraInfoDisplayed", (int)_cameraInfoDisplayed);
    main.setAttribute("cameraInfoPosition", (int)_cameraInfoPosition);
    main.setAttribute("cameraType", (int)_cameraType);
    main.appendChild(DM_DomUtils::QColorDomElement(m_normalColor, "normalColor", doc));
    main.setAttribute("normalLength", m_normalLength);

    return true;
}
