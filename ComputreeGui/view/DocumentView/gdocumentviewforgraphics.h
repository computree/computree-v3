#ifndef GDOCUMENTVIEWFORGRAPHICS_H
#define GDOCUMENTVIEWFORGRAPHICS_H

#include "gdocumentview.h"

#include "GraphicsViews/ggraphicsview.h"
#include "GraphicsViews/ggraphicsviewoptions.h"
#include "gcameragraphicsoptions.h"
#include "gpointofviewdocumentmanager.h"
#include "tools/attributes/worker/dm_attributesmanager.h"

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
    void addItemDrawable(ItemDrawable &item);
    void endAddMultipleItemDrawable();

    void beginRemoveMultipleItemDrawable();
    void removeItemDrawable(ItemDrawable &item);
    void endRemoveMultipleItemDrawable();

    void removeAllItemDrawableOfResult(Result &res);
    void removeAllItemDrawableOfModel(IItemModel &model);
    void removeAllSelectedItemDrawable();
    void removeAllItemDrawable();

    QList<InDocumentViewInterface*> views() const;
    void redrawGraphics();
    void lock();
    void unlock();

    bool acceptAction(const ActionInterface *action) const;
    bool setCurrentAction(ActionInterface *action, bool deleteAction = true);
    bool setDefaultAction(ActionInterface *action, bool deleteAction = true);
    ActionInterface* currentAction() const;
    ActionInterface* defaultAction() const;
    void removeActions(const QString &uniqueName) const;

    QString getType() const;

    template<typename Type>
    void createColorCloudRegistered() { }

    template<typename Type>
    void setColorCloudRegistered(QSharedPointer<ColorCloudRegisteredInterface> cc) { Q_UNUSED(cc) }

    template<typename Type>
    QSharedPointer<ColorCloudRegisteredInterface> colorCloudRegistered() const { return QSharedPointer<ColorCloudRegisteredInterface>(); }

    void setUseColorCloud(bool use);
    bool useColorCloud() const;

    template<typename Type>
    void setNormalCloudRegistered(QSharedPointer<NormalCloudRegisteredInterface> nn) { Q_UNUSED(nn) }

    template<typename Type>
    QSharedPointer<NormalCloudRegisteredInterface> normalCloudRegistered() const { return QSharedPointer<NormalCloudRegisteredInterface>(); }

    void setUseNormalCloud(bool use);
    bool useNormalCloud() const;

private:

    GGraphicsViewOptions                            *_graphicsOptionsView;
    GCameraGraphicsOptions                          *_cameraOptionsView;
    DM_AttributesManager                            m_attributesManager;
    QSharedPointer<ColorCloudRegisteredInterface>   m_pColorCloudRegistered;
    QSharedPointer<ColorCloudRegisteredInterface>   m_fColorCloudRegistered;
    QSharedPointer<ColorCloudRegisteredInterface>   m_eColorCloudRegistered;
    QSharedPointer<NormalCloudRegisteredInterface>  m_pNormalCloudRegistered;
    QSharedPointer<NormalCloudRegisteredInterface>  m_fNormalCloudRegistered;
    bool                                            m_useColorCloud;
    bool                                            m_useNormalCloud;

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
    void showPointsAttributesOptions();

    void changePixelSize();
    void changePixelSize(double size);
    void changeDrawMode();
    void changeDrawMode(DM_GraphicsViewOptions::DrawFastestMode mode);

private slots:

    void syncChanged(bool enable);
    void pluginExporterManagerReloaded();
    void exporterActionTriggered();

signals:

    void syncEnabled(const GDocumentViewForGraphics *view);
    void syncDisabled(const GDocumentViewForGraphics *view);
};


template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IPointAttributes>();
template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IFaceAttributes>();
template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IEdgeAttributes>();

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IPointAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc);
template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IFaceAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc);
template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IEdgeAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc);

template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IPointAttributes>() const;
template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IFaceAttributes>() const;
template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IEdgeAttributes>() const;

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<IPointAttributes>(QSharedPointer<NormalCloudRegisteredInterface> nn);
template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<IFaceAttributes>(QSharedPointer<NormalCloudRegisteredInterface> nn);

template<>
QSharedPointer<NormalCloudRegisteredInterface> GDocumentViewForGraphics::normalCloudRegistered<IPointAttributes>() const;
template<>
QSharedPointer<NormalCloudRegisteredInterface> GDocumentViewForGraphics::normalCloudRegistered<IFaceAttributes>() const;

#endif // GDOCUMENTVIEWFORGRAPHICS_H
