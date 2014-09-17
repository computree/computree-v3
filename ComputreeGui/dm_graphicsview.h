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

class DM_GraphicsView : public GraphicsViewInterface
{
    Q_INTERFACES(GraphicsViewInterface)

public:
    DM_GraphicsView();
    virtual ~DM_GraphicsView();

    virtual void setDocumentView(const DM_DocumentView *doc);
    DM_DocumentView& getDocumentView() const;

    /**
     * @brief Returns the document that contains this graphics view
     */
    DocumentInterface* document() const;

    virtual void setOptions(const DM_GraphicsViewOptions &newOptions);
    virtual GraphicsViewOptionsInterface& getOptions();

    virtual const DM_GraphicsViewOptions& getOptions() const;

    virtual void setCurrentPointCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc) = 0;
    virtual void setCurrentFaceCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc) = 0;
    virtual void setCurrentEdgeCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc) = 0;
    virtual void setCurrentPointCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn) = 0;
    virtual void setCurrentFaceCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn) = 0;

    virtual void setUseCloudColor(bool use) = 0;
    virtual void setUseCloudNormal(bool use) = 0;

    virtual void lockPaint() = 0;
    virtual void unlockPaint() = 0;

    virtual void redraw() = 0;

    virtual DM_GraphicsViewCamera* getCamera() const = 0;

protected:

    DM_DocumentView         *_document;
    DM_GraphicsViewOptions  *_options;
};

#endif // DM_GRAPHICSVIEW_H
