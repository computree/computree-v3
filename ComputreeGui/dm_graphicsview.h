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


#ifndef DM_GRAPHICSVIEW_H
#define DM_GRAPHICSVIEW_H

#include "dm_documentview.h"
#include "dm_graphicsviewoptions.h"
#include "dm_graphicsviewcamera.h"
#include "tools/attributes/worker/dm_attributesmanager.h"

class DM_GraphicsView : public GraphicsViewInterface
{
    Q_INTERFACES(GraphicsViewInterface)

public:

    enum RedrawOption {
        RO_NoOptions = 0x00,                    // No particular options
        RO_WaitForConversionCompleted = 0x02    // Pass this element to wait for conversion of item previously added/removed was completed
    };

    Q_DECLARE_FLAGS(RedrawOptions, RedrawOption)

    DM_GraphicsView();
    virtual ~DM_GraphicsView();

    virtual void setDocumentView(const DM_DocumentView *doc);
    DM_DocumentView* getDocumentView() const;

    void setAttributesManager(const DM_AttributesManager *manager);
    DM_AttributesManager* attributesManager() const;

    /**
     * @brief Returns the document that contains this graphics view
     */
    DocumentInterface* document() const;

    virtual void setOptions(const DM_GraphicsViewOptions &newOptions);
    virtual GraphicsViewOptionsInterface& getOptions();

    virtual const DM_GraphicsViewOptions& constGetOptionsInternal() const;

    virtual void lockPaint() = 0;
    virtual void unlockPaint() = 0;

    virtual void redraw(DM_GraphicsView::RedrawOptions opt = DM_GraphicsView::RO_NoOptions) = 0;

    virtual void updateDrawing3DOfItemDrawables(const QList<CT_AbstractItemDrawable*> &list) = 0;

    virtual void updateItemDrawablesThatColorWasModified() = 0;
    virtual void dirtyColorsOfItemDrawablesWithPoints() = 0;
    virtual void dirtyNormalsOfItemDrawablesWithPoints() = 0;

    virtual DM_GraphicsViewCamera* getCamera() const = 0;

    virtual void takeAndSaveScreenshot() = 0;

    virtual void getBoundingBoxOfAllItemDrawablePresentInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const = 0;
    virtual void getBoundingBoxOfAllItemDrawableSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const = 0;

    virtual void setVisible(const CT_AbstractItemDrawable *item, bool visible) = 0;
    virtual bool isVisible(const CT_AbstractItemDrawable *item) const = 0;

protected:

    DM_DocumentView         *_document;
    DM_GraphicsViewOptions  *_options;
    DM_AttributesManager    *m_attributesManager;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DM_GraphicsView::RedrawOptions)

#endif // DM_GRAPHICSVIEW_H
