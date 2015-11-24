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

    enum OctreeCellsConstructionType
    {
        OctreeCellsByNumber = 0,
        OctreeCellsBySize = 1
    };

    DM_GraphicsViewOptions();
    virtual ~DM_GraphicsViewOptions();

    // TODO : to remove
    int getFastDrawTime() const { return 0; }

    inline QColor getSelectedColor() const { return _selectedColor; }
    inline QColor getBackgroundColor() const { return _backgroundColor; }
    inline float getPointSize() const { return _pointSize; }
    inline bool drawAxis() const { return _drawAxis; }
    inline bool drawGrid() const { return m_drawGrid; }
    inline bool useColor() const { return _useColor; }
    inline bool useTransparency() const { return _useTransparency; }
    inline bool useLight() const { return _useLight; }
    inline DM_GraphicsViewOptions::CameraInfoDisplayed getCameraInformationDisplayed() const { return _cameraInfoDisplayed; }
    inline DM_GraphicsViewOptions::CameraInfoPosition getCameraInformationPosition() const { return _cameraInfoPosition; }
    inline CameraInterface::CameraType getCameraType() const { return _cameraType; }
    inline QColor normalColor() const { return m_normalColor; }
    inline float normalLength() const { return m_normalLength; }

    void setSelectedColor(QColor &color);
    void setBackgroudColor(QColor &color);
    void setPointSize(float size);
    void drawAxis(bool draw);
    void setDrawGrid(bool draw);
    void useColor(bool enable);
    void useTransparency(bool use);
    void useLight(bool use);
    void setCameraInformationDisplayed(DM_GraphicsViewOptions::CameraInfoDisplayed info);
    void setCameraInformationPosition(DM_GraphicsViewOptions::CameraInfoPosition pos);
    void setCameraType(CameraInterface::CameraType type);
    void setNormalColor(const QColor &color);
    void setNormalLength(float l);
    void updateFromOtherOptions(const DM_GraphicsViewOptions &options);

    bool load();
    bool save();

    bool loadFromXml(const QDomElement &el);
    bool saveToXml(QDomElement &main, QDomDocument& doc) const;

private:

    QColor                      _selectedColor;
    QColor                      _backgroundColor;
    float                       _pointSize;
    bool                        _drawAxis;
    bool                        m_drawGrid;
    bool                        _useColor;
    bool                        _useTransparency;
    bool                        _useLight;
    CameraInfoDisplayed         _cameraInfoDisplayed;
    CameraInfoPosition          _cameraInfoPosition;
    CameraInterface::CameraType _cameraType;
    QColor                      m_normalColor;
    float                       m_normalLength;

signals:

    void optionsChanged();
    void selectionColorChanged(const QColor &c);
    void normalColorChanged(const QColor &c);
    void normalLengthChanged(float f);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DM_GraphicsViewOptions::CameraInfoDisplayed)

#endif // DM_GRAPHICSVIEWOPTIONS_H
