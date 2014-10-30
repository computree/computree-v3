#ifndef GDOCUMENTVIEWFORGRAPHICS_H
#define GDOCUMENTVIEWFORGRAPHICS_H

#include "gdocumentview.h"

#include "dm_vertexvbomanager.h"

#include "GraphicsViews/ggraphicsview.h"
#include "GraphicsViews/ggraphicsviewoptions.h"
#include "gcameragraphicsoptions.h"
#include "gpointofviewdocumentmanager.h"
#include "tools/attributes/worker/dm_attributesmanager.h"

#include "GraphicsViews/3D/Octree/octreecontroller.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"

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

    QList<InDocumentViewInterface*> views() const;
    void redrawGraphics();
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
     * @brief Return the color of the item passed in parameter.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item);

    /**
     * @brief Returns true if this document use octree for points. By default return false.
     */
    virtual bool useOctreeOfPoints() const;

    /**
     * @brief Returns the octree of points or NULL if usePointsOctree() return false. By default return NULL.
     */
    virtual OctreeInterface* octreeOfPoints() const;

    template<typename Type>
    void createColorCloudRegistered() { }

    template<typename Type>
    void setColorCloudRegistered(QSharedPointer<CT_StandardColorCloudRegistered> cc) { Q_UNUSED(cc) }

    template<typename Type>
    QSharedPointer<CT_StandardColorCloudRegistered> colorCloudRegistered() const { return QSharedPointer<CT_StandardColorCloudRegistered>(); }

    void setUseColorCloud(bool use);
    bool useColorCloud() const;

    template<typename Type>
    void setNormalCloudRegistered(QSharedPointer<CT_StandardNormalCloudRegistered> nn) { Q_UNUSED(nn) }

    template<typename Type>
    QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudRegistered() const { return QSharedPointer<CT_StandardNormalCloudRegistered>(); }

    void setUseNormalCloud(bool use);
    bool useNormalCloud() const;

    /**
     * @brief Apply the attributes (scalar, color or normal for points/faces/edges) to array of this document.
     */
    void applyAttributes(DM_AbstractAttributes *dpa);

    /**
     * @brief Returns the unique vertex VBO manager
     */
    static DM_VertexVBOManager* staticUniqueVertexVBOManager() { return VERTEX_VBO_MANAGER; }

private:

    GGraphicsViewOptions                                            *_graphicsOptionsView;
    GCameraGraphicsOptions                                          *_cameraOptionsView;
    DM_AttributesManager                                            m_attributesManager;
    QSharedPointer<CT_StandardColorCloudRegistered>                 m_pColorCloudRegistered;
    QSharedPointer<CT_StandardColorCloudRegistered>                 m_fColorCloudRegistered;
    QSharedPointer<CT_StandardColorCloudRegistered>                 m_eColorCloudRegistered;
    QSharedPointer<CT_StandardNormalCloudRegistered>                m_pNormalCloudRegistered;
    QSharedPointer<CT_StandardNormalCloudRegistered>                m_fNormalCloudRegistered;
    bool                                                            m_useColorCloud;
    bool                                                            m_useNormalCloud;
    OctreeController                                                m_octreeController;
    DM_ColorVBOManager                                              *m_colorVboManager;

    static DM_VertexVBOManager                                      *VERTEX_VBO_MANAGER;
    static int                                                      N_DOCUMENT_VIEW_FOR_GRAPHICS;

    QList<GGraphicsView*>       _listGraphics;
    bool                        _graphicsLocked;
    QHBoxLayout                 *_layoutGraphics;

    QToolButton                 *_pointOfViewButton;
    GPointOfViewDocumentManager *_pointOfViewMenu;
    DM_PointOfViewManager       _pofManager;

    QPushButton                 *_buttonExport;
    QPushButton                 *_buttonPixelSize;
    QPushButton                 *_buttonDrawMode;
    QPushButton                 *_buttonConstructOctree;

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

    /**
     * @brief (Re)construct the octree. Do nothing by default.
     */
    virtual void constructOctreeOfPoints();

    void changePixelSize();
    void changePixelSize(double size);
    void changeDrawMode();
    void changeDrawMode(DM_GraphicsViewOptions::DrawFastestMode mode);
    void setTransparencyActivated(bool activated);

protected slots:

    virtual void slotItemDrawableAdded(CT_AbstractItemDrawable &item);
    virtual void slotItemToBeRemoved(CT_AbstractItemDrawable &item);
    virtual void closeEvent(QCloseEvent *closeEvent);

private slots:

    void syncChanged(bool enable);
    void pluginExporterManagerReloaded();
    void exporterActionTriggered();

signals:

    void syncEnabled(const GDocumentViewForGraphics *view);
    void syncDisabled(const GDocumentViewForGraphics *view);
};


template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractPointsAttributes>();
template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractFaceAttributes>();
template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractEdgeAttributes>();

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractPointsAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc);
template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractFaceAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc);
template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractEdgeAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc);

template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractPointsAttributes>() const;
template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractFaceAttributes>() const;
template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractEdgeAttributes>() const;

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<CT_AbstractPointsAttributes>(QSharedPointer<CT_StandardNormalCloudRegistered> nn);
template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<CT_AbstractFaceAttributes>(QSharedPointer<CT_StandardNormalCloudRegistered> nn);

template<>
QSharedPointer<CT_StandardNormalCloudRegistered> GDocumentViewForGraphics::normalCloudRegistered<CT_AbstractPointsAttributes>() const;
template<>
QSharedPointer<CT_StandardNormalCloudRegistered> GDocumentViewForGraphics::normalCloudRegistered<CT_AbstractFaceAttributes>() const;

#endif // GDOCUMENTVIEWFORGRAPHICS_H
