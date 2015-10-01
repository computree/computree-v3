/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GDOCUMENTMANAGERVIEW_H
#define GDOCUMENTMANAGERVIEW_H

#include "dm_documentmanagerview.h"

#include <QMdiArea>
#include <QMutex>
#include <QTimer>

class GDocumentView;
class GGraphicsViewSynchronizedGroup;

class GDocumentManagerView : public QMdiArea, public DM_DocumentManagerView, public DM_IDocumentCloseFilter, public DM_IDocumentAddFilter
{
    Q_OBJECT

public:
    explicit GDocumentManagerView(QWidget *parent = 0);
    ~GDocumentManagerView();

    void setSyncManager(const GGraphicsViewSynchronizedGroup *syncMan);

    bool addAllItemDrawableOfResultToActiveDocument(CT_AbstractResult &res, DM_AsynchroneProgress &progress);
    bool addAllItemDrawableOfModelToDocument(CT_AbstractResult &res, CT_OutAbstractItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress);
    bool addAllItemDrawableOfResultToDocument(CT_AbstractResult &res, DM_DocumentView *doc, DM_AsynchroneProgress &progress);

    bool addAllItemDrawableOfListToActiveDocument(QList<CT_AbstractItemDrawable*> &itemList, DM_AsynchroneProgress &progress);
    bool addAllItemDrawableOfListToDocument(QList<CT_AbstractItemDrawable*> &itemList, DM_DocumentView *doc, DM_AsynchroneProgress &progress);

    bool removeAllItemDrawableOfResultFromDocuments(CT_AbstractResult &res, DM_AsynchroneProgress &progress);
    bool removeAllItemDrawableOfModelFromDocuments(CT_OutAbstractItemModel &model, DM_AsynchroneProgress &progress);
    bool removeAllItemDrawableOfModelFromDocument(CT_OutAbstractItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress);
    bool removeAllItemDrawableOfListFromDocuments(QList<CT_AbstractItemDrawable *> &itemList, DM_AsynchroneProgress &progress);

    DM_DocumentView* getActiveDocumentView();
    DM_DocumentView* getDocumentView(int index) const;
    int nbDocumentView() const;
    bool containsDocument(DM_Document *document) const;

    void redrawAllDocument();

    DocumentInterface* new3DDocument(bool fromGui, bool inLoadConfigurationFromMainWindow);
    DocumentInterface* new2DDocument(bool fromGui, bool inLoadConfigurationFromMainWindow);
    DocumentInterface* newTreeViewDocument(bool fromGui, bool inLoadConfigurationFromMainWindow);

    // DocumentManagerInterface
    void lockAllDocuments();
    void unlockAllDocuments();

    DocumentInterface* new3DDocument();
    DocumentInterface* new2DDocument();
    DocumentInterface* newTreeViewDocument();
    int nDocuments() const;
    DocumentInterface* documentAt(const int &index) const;
    QList<DocumentInterface*> documents() const;
    DocumentInterface* activeDocument();
    void activateDocument(DocumentInterface *doc);
    bool closeDocument(DocumentInterface *doc);
    bool closeDocument(const int &index);

    // DM_IDocumentCloseFilter
    bool canClose(const DM_Document *document) const;

    // DM_IDocumentAddFilter
    bool canAddItemDrawable(const DM_Document *document, const CT_AbstractItemDrawable *item) const;

private:

    QMutex                          *m_mutex;
    int                             m_windowCount;
    QTimer                          m_timer;
    GGraphicsViewSynchronizedGroup  *m_syncMan;
    QMdiSubWindow                   *m_lastDocumentActivated;
    bool                            m_manualModeEnabled;
    CT_VirtualAbstractStep*         m_manualModeEnabledByStep;
    QList<GDocumentView*>           m_docAddedInNormalMode;
    QList<GDocumentView*>           m_docFromGuiAddedInManualMode;
    QList<GDocumentView*>           m_docToCloseAfterQuitManualMode;

    void addDocumentView(GDocumentView &view, bool fromGui, bool inLoadConfigurationFromMainWindow);
    QMdiSubWindow* subWindowFromDocument(DocumentInterface *doc) const;

public slots:
    void stepRequiredManualMode(CT_VirtualAbstractStep *step);
    void stepFinished(CT_VirtualAbstractStep *step);

signals:

    void documentAdded(DM_DocumentView *view);
    void documentToBeClosed(DM_DocumentView *view);
    void documentRemoved();
    void documentActivated(DM_DocumentView *view);
    void documentHidden(DM_DocumentView *view);
    void documentShowned(DM_DocumentView *view);

private slots:

    void slotDocumentDestroyed();
    void subWindowHasBeenActivated(QMdiSubWindow *window);
    void verifyNumberWindow();
};

#endif // GDOCUMENTMANAGERVIEW_H
