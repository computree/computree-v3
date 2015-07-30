#ifndef GDOCUMENTVIEWFORGRAPHICS_H
#define GDOCUMENTVIEWFORGRAPHICS_H

#include "gdocumentview.h"

#include "GraphicsViews/ggraphicsview.h"
#include "GraphicsViews/ggraphicsviewoptions.h"
#include "gcameragraphicsoptions.h"
#include "gpointofviewdocumentmanager.h"
#include "tools/attributes/worker/dm_attributesmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

#include "view/DocumentView/GraphicsViews/3D/gosggraphicsview.h"

#include <osg/Array>

#include <QHBoxLayout>
#include <QToolButton>

class GDocumentViewForGraphics : public GDocumentView
{
    Q_OBJECT

public:

    enum PixelSize {
        PX_1,
        PX_2,
        PX_3
    };


    GDocumentViewForGraphics(GDocumentManagerView &manager, QString title, QString type);
    virtual ~GDocumentViewForGraphics();

    void init();

    void addGraphics(GGraphicsView *graphics);
    const QList<GGraphicsView*>& getGraphicsList() const;

    void beginAddMultipleItemDrawable();
    void addItemDrawable(CT_AbstractItemDrawable &item);
    void endAddMultipleItemDrawable();

    void beginRemoveMultipleItemDrawable();
    void removeItemDrawable(CT_AbstractItemDrawable &item);
    void endRemoveMultipleItemDrawable();

    void removeAllItemDrawableOfResult(CT_AbstractResult &res);
    void removeAllItemDrawableOfModel(CT_OutAbstractModel &model);
    void removeAllSelectedItemDrawable();
    void removeAllItemDrawable();

    void updateDrawing3DOfItemDrawablesInGraphicsView(const QList<CT_AbstractItemDrawable*> &items);

    QList<InDocumentViewInterface*> views() const;
    void redrawGraphics();
    void dirtyColorsOfPoints();
    void lock();
    void unlock();

    bool acceptAction(const CT_AbstractAction *action) const;
    bool setCurrentAction(CT_AbstractAction *action, bool deleteAction = true);
    bool setDefaultAction(CT_AbstractAction *action, bool deleteAction = true);
    CT_AbstractAction* currentAction() const;
    CT_AbstractAction* defaultAction() const;
    void removeActions(const QString &uniqueName) const;

    QString getType() const;

    /**
     * @brief Returns true if this document use item color.
     */
    virtual bool useItemColor() const;

    /**
     * @brief Set the color of the item passed in parameter.
     */
    virtual void setColor(const CT_AbstractItemDrawable *item, const QColor &color);

    /**
     * @brief Returns true if the color of the itemdrawable passed in parameter is modified.
     */
    virtual bool isColorModified(const CT_AbstractItemDrawable *item);

    /**
     * @brief Return the color of the item passed in parameter.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item);

    /**
     * @brief Returns the global color array for points. If it don't exist it will be created before return it.
     */
    GOsgGraphicsView::ColorArrayType* getOrCreateGlobalColorArrayForPoints();
    CT_CCR getGlobalColorArrayRegisteredForPoints() const;

    /**
     * @brief Returns the global attrib array to set if a points is selected or not
     */
    typedef CT_StandardCloudOsgT<GLbyte, osg::Array::ByteArrayType, 1, GL_UNSIGNED_BYTE>     AttribCloudType;
    AttribCloudType::AType *getOrCreateGlobalAttribArrayForPoints();

    void setUseNormalCloud(bool use);
    bool useNormalCloud() const;

    /**
     * @brief Apply the attributes (scalar, color or normal for points/faces/edges) to array of this document.
     */
    void applyAttributes(DM_AbstractAttributes *dpa);

private:

    typedef CT_StdCloudRegisteredT< GDocumentViewForGraphics::AttribCloudType >               AttribCloudRegisteredType;


    GGraphicsViewOptions                                            *_graphicsOptionsView;
    GCameraGraphicsOptions                                          *_cameraOptionsView;

    DM_AttributesManager                                            m_attributesManager;

    bool                                                            m_useColorCloud;
    bool                                                            m_useNormalCloud;

    QTimer                                                          m_timerUpdateColors;
    QTimer                                                          m_timerDirtyColorsOfPoints;

    CT_CCR                                                          m_pointsColorCloudRegistered;
    QSharedPointer< AttribCloudRegisteredType >                     m_pointsAttribCloudRegistered;

    QList<GGraphicsView*>       _listGraphics;
    bool                        _graphicsLocked;
    QHBoxLayout                 *_layoutGraphics;

    QToolButton                 *_pointOfViewButton;
    GPointOfViewDocumentManager *_pointOfViewMenu;
    DM_PointOfViewManager       _pofManager;

    QPushButton                 *_buttonExport;
    QPushButton                 *_buttonPixelSize;
    QPushButton                 *_buttonDrawMode;

    QString                     _type;

    QMutex                      *m_mutex;

    DM_GraphicsViewOptions::DrawFastestMode     _drawMode;
    PixelSize                                   _pixelSize;

    void createAndAddCameraAndGraphicsOptions(QWidget *parent);
    void createAndAddItemDrawableWidgetContainer(QWidget *parent);
    void createAndAddGraphicsWidgetContainer(QWidget *parent);

    QString getKeyForPointOfViewManager();

    void lockGraphics();
    void unlockGraphics();

protected:

    /**
     * @brief Overloaded to create DM_ItemInfoForGraphics;
     */
    virtual DM_AbstractInfo* createNewItemInformation(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Recursive set color of childrens of this group
     */
    void recursiveSetColor(CT_AbstractItemGroup *group,
                              const QColor &color);

public slots:

    /**
      * \brief Affiche les options du document
      */
    void showOptions();
    void validateOptions();

    /**
     * @brief Save a screenshot to disk
     */
    void takeAndSaveScreenshot();

    /**
      * \brief Ajoute le point de vue actuel du graphique au gestionnaire de point de vue
      */
    void addActualPointOfView();

    /**
      * \brief Dfini le point de vue pass en paramtre au graphique du document
      */
    void setPointOfView(DM_PointOfView *pof);

    /**
     * @brief Show points attributes manager
     */
    void showAttributesOptions();

    void changePixelSize();
    void changePixelSize(double size);
    void changeDrawMode();
    void changeDrawMode(DM_GraphicsViewOptions::DrawFastestMode mode);
    void setTransparencyActivated(bool activated);
    void setCameraType(bool orthographic);


protected slots:
    virtual void closeEvent(QCloseEvent *closeEvent);

private slots:

    void syncChanged(bool enable);
    void pluginExporterManagerReloaded();
    void exporterActionTriggered();
    void mustUpdateItemDrawablesThatColorWasModified();
    void mustDirtyColorsOfItemDrawablesWithPoints();

signals:

    void syncEnabled(const GDocumentViewForGraphics *view);
    void syncDisabled(const GDocumentViewForGraphics *view);

    void startDirtyColorsOfPointTimer();
    void startUpdateColorsTimer();
};

#endif // GDOCUMENTVIEWFORGRAPHICS_H
