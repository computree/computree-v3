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

#ifndef G3DGRAPHICSVIEW_H
#define G3DGRAPHICSVIEW_H

#include "GraphicsViews/ggraphicsview.h"
#include "GraphicsViews/3D/g3dpainter.h"
#include "GraphicsViews/3D/g3dfakepainter.h"
#include "GraphicsViews/3D/g3dcameracontroller.h"

#include "dm_fastestincrementoptimizer.h"

#include <qglviewer.h>

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"

class GDocumentViewForGraphics;
class G3DGraphicsView;
class DM_ElementInfoManager;

class G3DGraphicsViewSignalEmitter : public GraphicsViewSignalEmitterInterface
{
public:
    void setGraphicsView(const G3DGraphicsView *gv);
    GraphicsViewInterface* graphicsView() const;

    void emitDrawingStarted();
    void emitDrawingFinished();

protected:
    G3DGraphicsView *m_graphicsView;

};

class G3DGraphicsView : public QGLViewer, public GGraphicsView
{
    Q_OBJECT

public:
    G3DGraphicsView(QWidget *parent);
    virtual ~G3DGraphicsView();

    QWidget* getViewWidget() const;

    void setDocumentView(const DM_DocumentView *doc);

    void setOptions(const DM_GraphicsViewOptions &newOptions);

    GraphicsViewOptionsInterface& getOptions();
    const DM_GraphicsViewOptions& getOptions() const;

    // Inherit from GraphicsViewInterface
    GraphicsViewSignalEmitterInterface* signalEmitter() const;
    QSharedPointer<CT_StandardColorCloudRegistered> colorCloudOf(ColorCloudType type) const;
    QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudOf(NormalCloudType type) const;
    GraphicsViewType type() const;

    bool mustSelectPoints() const;
    bool mustSelectFaces() const;
    bool mustSelectEdges() const;
    bool mustSelectItems() const;

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedPoints() const;
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedFaces() const;
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedEdges() const;
    QList<CT_AbstractItemDrawable*> getSelectedItems() const;

    void beginRemoveMultiplePointsFromSelection(const size_t &n);
    void removePointFromSelection(const size_t &globalIndex);
    void endRemoveMultiplePointsFromSelection();
    void setAllPointsSelected(bool select);

    void beginRemoveMultipleFacesFromSelection(const size_t &n);
    void removeFaceFromSelection(const size_t &globalIndex);
    void endRemoveMultipleFacesFromSelection();
    void setAllFacesSelected(bool select);

    void beginRemoveMultipleEdgesFromSelection(const size_t &n);
    void removeEdgeFromSelection(const size_t &globalIndex);
    void endRemoveMultipleEdgesFromSelection();
    void setAllEdgesSelected(bool select);

    size_t countPoints();
    size_t countEdges();
    size_t countFaces();
    size_t countItems();

    void setDrawMode(DrawMode dMode);
    GraphicsViewInterface::DrawMode drawMode() const;
    GraphicsViewInterface::DrawMode drawModeToUse() const;
    void setDrawModeChangeTime(const int &msec);
    int drawModeChangeTime() const;

    void setSelectBufferSize(size_t size);
    size_t selectBufferSize() const;
    void setSelectRegionWidth(int width);
    void setSelectRegionHeight(int height);
    void setSelectionMode(SelectionMode mode);
    GraphicsViewInterface::SelectionMode selectionMode() const;
    void select(const QPoint& point);

    bool getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const;
    float distanceToFrustumPlane(int index, const double &x, const double &y, const double &z) const;
    bool aaBoxIsVisible(const QVector3D& p1, const QVector3D& p2, bool *entirely) const;
    bool sphereIsVisible(const QVector3D& center, float radius) const;

    QVector3D pointUnderPixel(const QPoint &pixel, bool &found) const;
    void convertClickToLine(const QPoint &pixel, QVector3D &orig, QVector3D &dir) const;
    void addActionOptions(ActionOptionsInterface *options);
    void removeActionOptions(ActionOptionsInterface *options);
    int width() const;
    int height() const;

    void lockPaint();
    void unlockPaint();

    void redraw();

    void setDrawFastest(DM_GraphicsViewOptions::DrawFastestMode mode);
    DM_GraphicsViewOptions::DrawFastestMode drawFastest() const;
    bool mustDrawFastestNow() const;

    void setUseCloudColor(bool use);
    void setUseCloudNormal(bool use);

    CameraInterface* camera() const { return getCamera(); }
    DM_GraphicsViewCamera* getCamera() const;

    void initFromOptions();
    void active2dView(bool enable);
    bool is2DViewActived() const;

    void showContextMenu(const QPoint &pos);

    DM_ElementInfoManager* pointsInformationManager() const;
    DM_ElementInfoManager* facesInformationManager() const;
    DM_ElementInfoManager* edgesInformationManager() const;

private:

    GDocumentViewForGraphics        *m_docGV;
    QMutex                          *_mutex;
    QPainter                        *m_painter;
    G3DPainter                      _g;
    G3DFakePainter                  m_fakeG;
    G3DCameraController             _cameraController;
    SelectionMode                   _selectionMode;
    DrawMode                        _drawModeToUse;
    DrawMode                        _drawModeUsed;
    QTimer                          _timerChangeDrawMode;
    bool                            _2dActive;
    bool                            m_useColorCloud;
    bool                            _forceDrawMode;
    G3DGraphicsViewSignalEmitter    m_signalEmitter;
    GLdouble                        m_planeCoefficients[6][4];
    QPoint                          m_centerPointOfSelection;

    QList<CT_AbstractCloudIndex*>   m_idToAddInSelection;

    DM_ElementInfoManager           *m_pointsSelectionManager;
    DM_ElementInfoManager           *m_facesSelectionManager;
    DM_ElementInfoManager           *m_edgesSelectionManager;

    DM_FastestIncrementOptimizer    m_fastestIncrementOptimizer;
    DM_ColorVBOManager              *m_colorVboManager;

    GLint viewport_[4];
    GLdouble modelViewMatrix_[16];
    GLdouble projectionMatrix_[16];


    void addIdToSelection(const GLuint &id);
    void addPointsIDToSelection(const GLuint &id);
    void addFacesIDToSelection(const GLuint &id);
    void addEdgesIDToSelection(const GLuint &id);
    void addItemsIDToSelection(const GLuint &id);

    void setLastItemIdSelected(const GLuint &id);
    void setLastPointIdSelected(const GLuint &id);
    void setLastFaceIdSelected(const GLuint &id);
    void setLastEdgeIdSelected(const GLuint &id);

    void removeIdFromSelection(const GLuint &id);
    void removePointsIDFromSelection(const GLuint &id);
    void removeFacesIDFromSelection(const GLuint &id);
    void removeEdgesIDFromSelection(const GLuint &id);
    void removeItemsIDFromSelection(const GLuint &id);

    void removeAllIdFromSelection();

    void drawCoordinates(QPainter &painter);
    void changeStateFileName();

    SelectionMode selectionModeToBasic() const;

    void checkAndShowOpenGLErrors();

    void startRedrawTimer();

    QMenu* constructContextMenu() const;

    template<typename CT_TypeAttributes>
    void constructContextMenuAction(QMenu *menu, CT_VirtualAbstractStep *st) const;

protected:
    void internalSetDrawMode(DrawMode dMode);

    void init();
    void initOptions();
    void preDraw();
    void draw();
    void postDraw();
    void fastDraw();
    void drawInternal();
    void drawOverlay(QPainter &painter);
    void drawWithNames();

    virtual void paintGL() { update(); }
    virtual void paintEvent(QPaintEvent *e);

    void endSelection(const QPoint&);
    void postSelection(const QPoint& point);

    bool event(QEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

signals:

    void mustRedraw();
    void mustSetDrawModeStartTimerAndRedraw();

private slots:

    void setDrawModeStartTimerAndRedraw();
    void changeDrawMethodToNormal();
    void itemDrawableToBeRemoved(CT_AbstractItemDrawable &item);
    void applyAttributes();
};

#endif // G3DGRAPHICSVIEW_H
