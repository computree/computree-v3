#ifndef GOSGGRAPHICSVIEW_H
#define GOSGGRAPHICSVIEW_H

#include <QMutex>
#include <qglobal.h>


//#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
#if (QT_MAJOR_VERSION <= 5 && QT_MINOR_VERSION < 4)
#include <QGLWidget>
#define Q_GL_WIDGET QGLWidget
#else
#include <QOpenGLWidget>
#define Q_GL_WIDGET QOpenGLWidget
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QGLFunctions>
#define Q_GL_FUNCTIONS QGLFunctions
#define Q_GL_FUNCTIONS_INIT initializeGLFunctions
#else
#include <QOpenGLFunctions>
#define Q_GL_FUNCTIONS QOpenGLFunctions
#define Q_GL_FUNCTIONS_INIT initializeOpenGLFunctions
#endif

#include <osgViewer/CompositeViewer>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Node>
#include <osgText/Text>

#include "view/DocumentView/GraphicsViews/ggraphicsview.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"
#include "view/DocumentView/GraphicsViews/3D/Camera/g3dcameracontroller.h"

class DM_OsgSceneManager;
class DM_OsgPicker;
class GOsgGraphicsView;

/**
 * @brief Class to emit signals of graphicsView
 */
class GOsgGraphicsViewSignalEmitter : public GraphicsViewSignalEmitterInterface
{
public:
    void setGraphicsView(GOsgGraphicsView *gv) { m_graphicsView = gv; }
    GraphicsViewInterface* graphicsView() const;

    void emitDrawingStarted() { emit drawingStarted(); }
    void emitDrawingFinished() { emit drawingFinished(); }

protected:
    GOsgGraphicsView *m_graphicsView;
};

/**
 * @brief Class used to save the screenshot
 */
class GOsgCaptureOperation : public osgViewer::ScreenCaptureHandler::CaptureOperation
{
public:
    void operator()(const osg::Image& image, const unsigned int context_id);
};

class GOsgFindPointUnderPixelCallBack : public osg::Camera::DrawCallback
{
public:
    GOsgFindPointUnderPixelCallBack(QPoint pixel);

    virtual void operator () (osg::RenderInfo& renderInfo) const;

    bool hasFound() const;

    Eigen::Vector3d foundedPoint() const;

private:
    GLfloat         m_alpha;
    QPoint          m_pixel;
    Eigen::Vector3d m_point;
};

/**
 * @brief Graphics view that use the GLWidget of OpenSceneGraph to manage the scene
 */
class GOsgGraphicsView : public Q_GL_WIDGET, public GGraphicsView, protected Q_GL_FUNCTIONS
{
    Q_OBJECT
public:

    /**
     * @brief Type of color array used by this view
     */
    typedef DM_PainterToOsgElements::ColorArrayType     ColorArrayType;

    /**
     * @brief Type of normal array used by this view
     */
    typedef DM_PainterToOsgElements::NormalArrayType     NormalArrayType;


    explicit GOsgGraphicsView(QWidget *parent = 0);
    ~GOsgGraphicsView();

    /**
     * @brief Must be called after constructor
     */
    void init();

    /**
     * @brief Calle this method to active/desactive the 2D view (view in orthographics mode constrained)
     * @param enable
     */
    void active2dView(bool enable);

    /**
     * @brief Returns the composite viewer
     */
    osgViewer::CompositeViewer* getCompositeViewer() const;

    /**
     * @brief Change the camera type if this view is not 2D
     */
    void setCameraType(CameraInterface::CameraType type);

    /**
     * @brief Return true if the camera is orthographic
     */
    bool isOrthographicCamera() const;

    /**
     * @brief Return true if the camera is perspective
     */
    bool isPerspectiveCamera() const;

    // -------- GGraphicsView ---------- //
    /**
     * @brief Returns the widget that represents this graphics to attach it to a layout per example
     */
    QWidget* getViewWidget() const;

    /**
     * @brief Returns true if this graphics view is 2D
     */
    bool is2DView() const;

    // -------- DM_GraphicsView ---------- //

    void setOptions(const DM_GraphicsViewOptions &newOptions);

    /**
     * @brief Lock the paint (if you add or remove items in a thread...)
     */
    void lockPaint();

    /**
     * @brief Unlock the paint after locked it please...
     */
    void unlockPaint();

    /**
     * @brief Called when we must redraw
     */
    void redraw(DM_GraphicsView::RedrawOptions opt = DM_GraphicsView::RO_NoOptions);

    /**
     * @brief Returns the camera used by this graphics view
     */
    DM_GraphicsViewCamera* getCamera() const;

    /**
     * @brief Call this method to take and screenshot and show a messagebox at user to save it
     */
    void takeAndSaveScreenshot();

    /**
     * @brief Call this method to update the 3d drawing of itemdrawables passed in parameter, per example if you change a parameter in it's draw configuration
     */
    void updateDrawing3DOfItemDrawables(const QList<CT_AbstractItemDrawable*> &list);

    /**
     * @brief Call this method if you have changed colors of items in the document and you want that in view it will be
     *        updated
     */
    void updateItemDrawablesThatColorWasModified();

    /**
     * @brief Call this method if you want to change the color of points draw by the item passed in parameter
     */
    void updateColorOfPointsOfItemDrawable(CT_AbstractItemDrawable *item, const QColor &color);

    /**
     * @brief Call this method if you have changed colors of points in the document (colorArray) and you want that in view it will
     *        be updated
     */
    void dirtyColorsOfItemDrawablesWithPoints();

    /**
     * @brief Call this method if you have changed normals of points in the document (normalArray) and you want that in view it will
     *        be updated
     */
    void dirtyNormalsOfItemDrawablesWithPoints();

    /**
     * @brief Change the color of the point. You must call dirtyColorsOfItemDrawablesWithPoints() if you want that change was visible.
     * @param globalIndexOfPoint : the global index of the point ot change his color
     * @param color : the new color
     */
    void setColorOfPoint(const size_t &globalIndexOfPoint, const CT_Color &color);

    /**
     * @brief Returns the color of the point.
     * @warning if you change the value of the color you must call dirtyColorsOfItemDrawablesWithPoints() to update it.
     * @param globalIndexOfPoint : the global index of the point
     * @return the color of the point
     */
    CT_Color& colorOfPoint(const size_t &globalIndexOfPoint);

    /**
     * @brief Returns the color of the point.
     * @param globalIndexOfPoint : the global index of the point
     * @return the color of the point
     */
    const CT_Color& constColorOfPoint(const size_t &globalIndexOfPoint) const;

    /**
     * @brief Change the normal of the point. You must call dirtyNormalsOfItemDrawablesWithPoints() if you want that change was visible.
     * @param globalIndexOfPoint : the global index of the point ot change his normal
     * @param normal : the new normal
     */
    void setNormalOfPoint(const size_t &globalIndexOfPoint, const CT_Normal &normal);

    /**
     * @brief Returns the normal of the point.
     * @warning if you change the value of the normal you must call dirtyNormalsOfItemDrawablesWithPoints() to update it.
     * @param globalIndexOfPoint : the global index of the point
     * @return the normal of the point
     */
    CT_Normal& normalOfPoint(const size_t &globalIndexOfPoint);

    /**
     * @brief Returns the normal of the point.
     * @param globalIndexOfPoint : the global index of the point
     * @return the normal of the point
     */
    const CT_Normal& constNormalOfPoint(const size_t &globalIndexOfPoint) const;

    /**
     * @brief Returns the bounding box of all item drawable present in the view
     */
    void getBoundingBoxOfAllItemDrawablePresentInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    /**
     * @brief Returns the bounding box of all item drawable selected in the view
     */
    void getBoundingBoxOfAllItemDrawableSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    // -------- GraphicsViewInterface ---------- //

    /**
     * @brief Return the signal emitter of the graphics view
     */
    virtual GraphicsViewSignalEmitterInterface* signalEmitter() const;

    /**
     * @brief Return the color cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardColorCloudRegistered> colorCloudOf(ColorCloudType type) const;

    /**
     * @brief Return the normal cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudOf(NormalCloudType type) const;

    /**
     * @brief Return the camera of the view
     */
    virtual CameraInterface* camera() const;

    /**
     * @brief Return the type of the view
     */
    virtual GraphicsViewType type() const;

    /**
     * @brief Set the selection width. Default is 3;
     */
    virtual void setSelectRegionWidth(int width);

    /**
     * @brief Set the selection height. Default is 3;
     */
    virtual void setSelectRegionHeight(int height);

    /**
     * @brief Set the selection mode
     */
    virtual void setSelectionMode(SelectionMode mode);

    /**
     * @brief Get the selection mode
     */
    virtual GraphicsViewInterface::SelectionMode selectionMode() const;

    /**
     * @brief Returns true if the view must select points
     */
    virtual bool mustSelectPoints() const;

    /**
     * @brief Returns true if the view must select edges
     */
    virtual bool mustSelectEdges() const;

    /**
     * @brief Returns true if the view must select faces
     */
    virtual bool mustSelectFaces() const;

    /**
     * @brief Returns true if the view must select items
     */
    virtual bool mustSelectItems() const;

    /**
     * @brief Performs a selection in the scene from pixel coordinates.
     */
    virtual void select(const QPoint& point);

    /**
     * @brief Returns all points (indices) selected. Can be NULL if no points where selected.
     */
    virtual CT_SPCIR getSelectedPoints() const;

    /**
     * @brief Returns all faces (indices) selected. Can be NULL if no faces where selected.
     */
    virtual CT_SFCIR getSelectedFaces() const;

    /**
     * @brief Returns all edges (indices) selected. Can be NULL if no edges where selected.
     */
    virtual CT_SECIR getSelectedEdges() const;

    /**
     * @brief Returns all ItemDrawable selected
     */
    virtual QList<CT_AbstractItemDrawable*> getSelectedItems() const;

    /**
     * @brief Add a point to selection. Pass the global index of the point.
     */
    virtual void addPointsIDToSelection(const size_t &id);

    /**
     * @brief Add multiple point to selection. Pass a vector of global index of points to add.
     */
    virtual void addMultiplePointsIDToSelection(const std::vector<size_t> &idList);

    /**
     * @brief Add a face to selection. Pass the global index of the face.
     */
    virtual void addFacesIDToSelection(const size_t &id);

    /**
     * @brief Add multiple face to selection. Pass a vector of global index of faces to add.
     */
    virtual void addMultipleFacesIDToSelection(const std::vector<size_t> &idList);

    /**
     * @brief Add a edge to selection. Pass the global index of the point.
     */
    virtual void addEdgesIDToSelection(const size_t &id);

    /**
     * @brief Add multiple edge to selection. Pass a vector of global index of edges to add.
     */
    virtual void addMultipleEdgesIDToSelection(const std::vector<size_t> &idList);

    /**
     * @brief Remove a point to selection. Pass the global index of the point.
     */
    virtual void removePointsIDFromSelection(const size_t &id);

    /**
     * @brief Remove multiple point to selection. Pass a vector of global index of points to remove.
     */
    virtual void removeMultiplePointsIDFromSelection(const std::vector<size_t> &idList);

    /**
     * @brief Remove a face to selection. Pass the global index of the face.
     */
    virtual void removeFacesIDFromSelection(const size_t &id);

    /**
     * @brief Remove multiple face to selection. Pass a vector of global index of faces to remove.
     */
    virtual void removeMultipleFacesIDFromSelection(const std::vector<size_t> &idList);

    /**
     * @brief Remove a edge to selection. Pass the global index of the point.
     */
    virtual void removeEdgesIDFromSelection(const size_t &id);

    /**
     * @brief Remove multiple edge to selection. Pass a vector of global index of edges to remove.
     */
    virtual void removeMultipleEdgesIDFromSelection(const std::vector<size_t> &idList);

    /**
     * @brief Call this method with true if you want to select all points, false if you want to unselect all points
     */
    virtual void setAllPointsSelected(bool select);

    /**
     * @brief Call this method with true if you want to select all faces, false if you want to unselect all faces
     */
    virtual void setAllFacesSelected(bool select);

    /**
     * @brief Call this method with true if you want to select all edges, false if you want to unselect all edges
     */
    virtual void setAllEdgesSelected(bool select);

    /**
     * @brief Returns how many points is drawn
     */
    virtual size_t countPoints();

    /**
     * @brief Returns how many edges is drawn
     */
    virtual size_t countEdges();

    /**
     * @brief Returns how many faces is drawn
     */
    virtual size_t countFaces();

    /**
     * @brief Returns how many items is drawn
     */
    virtual size_t countItems();

    /**
     * @brief Returns the coordinates of the 3D point located at pixel (x,y) on screen.
     *
     * Calls a glReadPixel to get the pixel depth and applies an unprojectedCoordinatesOf() to
     * the result. found indicates whether a point was found or not (i.e. background pixel, result's
     * depth is zFar() in that case).
     *
     * x and y are expressed in pixel units with an origin in the upper left corner.
     */
    virtual Eigen::Vector3d pointUnderPixel(const QPoint &pixel, bool &found) const;

    /**
     * @brief Gives the coefficients of a 3D half-line passing through the Camera eye and pixel (x,y).
     *
     * The origin of the half line (eye position) is stored in orig, while dir contains the properly oriented and normalized direction of the half line.
     *
     * x and y are expressed in Qt format (origin in the upper left corner).
     *
     * This method is useful for analytical intersection in a selection method.
     */
    virtual void convertClickToLine(const QPoint &pixel, Eigen::Vector3d &orig, Eigen::Vector3d &dir) const;

    /**
     * @brief Returns the screen projected coordinates (pixel) of the variable position.
     */
    virtual void convert3DPositionToPixel(const Eigen::Vector3d &position, QPoint &pixel) const;

    /**
     * @brief Return the width of the graphics
     */
    virtual int width() const;

    /**
     * @brief Return the height of the graphics
     */
    virtual int height() const;

    /**
     * @brief Change the visibility of the item in the document
     */
    virtual void setVisible(const CT_AbstractItemDrawable *item, bool visible);

    /**
     * @brief Returns the visibility of the item in the document.
     */
    virtual bool isVisible(const CT_AbstractItemDrawable *item) const;

protected:

    void resizeGL(int width, int height);
    void onResize(int width, int height);
    osgGA::EventQueue* getEventQueue() const;

    void initializeGL();

    void paintEvent( QPaintEvent* paintEvent );

    void paintGL();
    void paintOverlay(QPainter &painter);
    void paintOverlayCameraInformation(QPainter &painter);

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    bool event( QEvent* event );

protected:

    friend class DM_OsgSceneManager;

    /**
     * @brief Called from osg scene manager when conversion is beginning
     */
    void conversionBegin();

    /**
     * @brief Called from osg scene manager when conversion is completed
     */
    void conversionCompleted();

private:
    friend class GOsgFindPointUnderPixelCallBack;
    QMutex                                              *m_mutex;

    GOsgGraphicsViewSignalEmitter                       m_signalEmitter;
    bool                                                m_mouseButtonPressed;   // true if mouse button is pressed in the view (for method eventFilter)

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded>     m_graphicsWindow;
    osg::ref_ptr<osgViewer::CompositeViewer>            m_compositeViewer;

    osgViewer::View                                     *m_view;                // the view that draw the scene
    osg::ref_ptr<osg::Group>                            m_scene;                // the scene root

    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>     m_manSwitch;            // manipulator swith : TrackBall manipulator or 2D manipulator

    DM_OsgSceneManager                                  *m_sceneManager;        // the scene manager
    DM_OsgPicker                                        *m_picker;              // the picker
    G3DCameraController                                 m_camController;        // element that control the real camera

    osgViewer::ScreenCaptureHandler                     *m_captureScreenHandler;

    osg::ref_ptr<osg::Node>                             m_axis;

    QString                                             m_defaultStateFileName;
    uint                                                m_uniqueIndex;

    osg::ref_ptr<osg::Camera>                           m_hudCamera;

    bool                                                m_conversionInProgress;

    static uint                                         NUMBER_OF_VIEWS;

    /**
     * @brief Stop the spinning of the manipulator !
     */
    void stopSpinning();

    /**
     * @brief Call to update the text that shows informations
     */
    void updateInformations();

    /**
     * @brief Generate the state filename for this view
     */
    QString stateFileName() const;

    /**
     * @brief Save configuration of the view in a file
     */
    void saveStateToFile();

    /**
     * @brief Restore configuration view from file
     */
    bool restoreStateFromFile();

signals:
    /**
     * @brief Emitted when the conversion of itemdrawable added was completed
     */
    void internalStopWaitingForConversionCompleted();

    /**
     * @brief Emitted when an itemdrawable was added to this view
     */
    void itemDrawableAdded(CT_AbstractItemDrawable &item);

    /**
     * @brief Emitted when an itemdrawable was removed from this view
     */
    void itemDrawableRemoved(CT_AbstractItemDrawable &item);
};

#endif // GOSGGRAPHICSVIEW_H
