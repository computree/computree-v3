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


#ifndef DM_GRAPHICSVIEWOPTIONS_H
#define DM_GRAPHICSVIEWOPTIONS_H

#include "interfaces.h"

#include <QColor>
#include <QDomElement>

class DM_GraphicsViewOptions : public QObject, public GraphicsViewOptionsInterface
{
    Q_OBJECT
    Q_INTERFACES(GraphicsViewOptionsInterface)

public:

    enum DrawFastestMode
    {
        Always = 0,
        Normal = 1,
        Never = 2
    };

    enum CameraInfoFlags
    {
        CameraNone = 0x0,
        CameraPosition = 0x1,
        CameraSceneCenter = 0x2,
        CameraViewDirection = 0x4,
        FpsInformation = 0x8
    };

    Q_DECLARE_FLAGS(CameraInfoDisplayed, CameraInfoFlags)

    enum CameraInfoPosition
    {
        InfoInUpperLeftCorner,
        InfoInUpperRightCorner,
        InfoInLowerRightCorner,
        InfoInLowerLeftCorner
    };

    DM_GraphicsViewOptions();
    virtual ~DM_GraphicsViewOptions();

    inline QColor getSelectedColor() const { return _selectedColor; }
    inline QColor getBackgroundColor() const { return _backgroundColor; }
    inline float getPointSize() const { return _pointSize; }
    inline bool drawAxis() const { return _drawAxis; }
    inline bool useColor() const { return _useColor; }
    inline DM_GraphicsViewOptions::DrawFastestMode drawFastest() const { return _drawFastest; }
    inline bool useTransparency() const { return _useTransparency; }
    inline bool useLight() const { return _useLight; }
    inline int getFastDrawTime() const { return _fastDrawTime; }
    inline int getMinFPS() const { return m_minFPS; }
    inline DM_GraphicsViewOptions::CameraInfoDisplayed getCameraInformationDisplayed() const { return _cameraInfoDisplayed; }
    inline DM_GraphicsViewOptions::CameraInfoPosition getCameraInformationPosition() const { return _cameraInfoPosition; }
    inline CameraInterface::CameraType getCameraType() const { return _cameraType; }
    inline bool showOctree() const { return m_showOctree; }
    inline int octreeNumberOfCells() const { return m_octreeNumberOfCells; }

    void setSelectedColor(QColor &color);
    void setBackgroudColor(QColor &color);
    void setPointSize(float size);
    void drawAxis(bool draw);
    void useColor(bool enable);
    void useTransparency(bool use);
    void useLight(bool use);
    void drawFastest(DM_GraphicsViewOptions::DrawFastestMode mode);
    void setFastDrawTime(int time);
    void setMinFPS(int fps);
    void setCameraInformationDisplayed(DM_GraphicsViewOptions::CameraInfoDisplayed info);
    void setCameraInformationPosition(DM_GraphicsViewOptions::CameraInfoPosition pos);
    void setCameraType(CameraInterface::CameraType type);
    void setShowOctree(bool val);
    void setOctreeNumberOfCells(int n);
    void updateFromOtherOptions(const DM_GraphicsViewOptions &options);

    bool load();
    bool save();

    bool loadFromXml(QDomElement &el);
    bool saveToXml(QDomElement &main, QDomDocument& doc) const;

private:

    QColor                      _selectedColor;
    QColor                      _backgroundColor;
    float                       _pointSize;
    bool                        _drawAxis;
    bool                        _useColor;
    DrawFastestMode             _drawFastest;
    bool                        _useTransparency;
    bool                        _useLight;
    int                         _fastDrawTime;
    CameraInfoDisplayed         _cameraInfoDisplayed;
    CameraInfoPosition          _cameraInfoPosition;
    CameraInterface::CameraType _cameraType;
    bool                        m_showOctree;
    int                         m_octreeNumberOfCells;
    int                         m_minFPS;

signals:

    void optionsChanged();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DM_GraphicsViewOptions::CameraInfoDisplayed)

#endif // DM_GRAPHICSVIEWOPTIONS_H
