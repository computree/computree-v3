#ifndef DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H
#define DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H

#include <QHash>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/3D/Converter/dm_singleitemdrawabletoosgworker.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_geometriesconfiguration.h"
#include "view/DocumentView/gdocumentviewforgraphics.h"

/**
 * @brief This class will convert all itemdrawable passed in parameter to osg element if you want to add it in
 *        a scene. The conversion is made in multithread (async) !
 */
class DM_MultipleItemDrawableToOsgWorker : public QObject
{
    Q_OBJECT

public:
    DM_MultipleItemDrawableToOsgWorker(GOsgGraphicsView &view);
    ~DM_MultipleItemDrawableToOsgWorker();

    /**
     * @brief Returns true if a conversion is in progress or in wait to begin
     */
    bool isConversionInProgress() const;

    /**
     * @brief Return results of this worker and clear it
     */
    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > takeResults();

    /**
     * @brief Returns the location index of the vertex attrib array set to global geometries
     */
    static uint staticGlobalVertexAttribArrayLocationIndex();

    /**
     * @brief Returns the location index of the vertex attrib array set to local geometries
     */
    static uint staticLocalVertexAttribArrayLocationIndex();

    /**
     * @brief Returns a new new Shader(Program) initialized to draw points with selected colors, etc...
     */
    osg::ref_ptr<osg::Program> createFirstPassForGlobalPoints() const;

    /**
     * @brief Returns a new Shader(Program) initialized to draw normals of points
     */
    osg::ref_ptr<osg::Program> createSecondPassForGlobalPoints() const;

    /**
     * @brief Get geometries configuration to set
     */
    DM_GeometriesConfiguration getGeometriesConfiguration() const;

private:
    GOsgGraphicsView                                                        &m_view;

    QHash<CT_AbstractItemDrawable*, DM_SingleItemDrawableToOsgWorker*>      m_toConvert;
    QList<DM_SingleItemDrawableToOsgWorker*>                                m_toConvertTemporary;
    QFuture<void>                                                           m_future;
    QFutureWatcher<void>                                                    m_futureWatcher;
    DM_AsyncOperation                                                       *m_aop;

    DM_GeometriesConfiguration                                              m_geometriesConfiguration;

    QMutex                                                                  *m_mutex;
    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult >        m_results;
    bool                                                                    m_stop;

    QTimer                                                                  m_timerComputeQueue;

    void initGeometriesConfigurationForGlobalElements();
    void initGeometriesConfigurationForLocalElements();

    /**
     * @brief Static method called by QtConcurrent to execute the worker passed in parameter in a thread
     */
    static void staticComputeQtConcurrent(DM_SingleItemDrawableToOsgWorker *worker);

    /**
     * @brief Static method to create the global color array (for points) only on demand. Pass this function to a DM_GeometriesConfiguration
     */
    static DM_PainterToOsgElements::ColorArrayType* staticGetOrCreateColorArrayFunction(void *context);

    /**
     * @brief Static method to create the global normal array (for points) only on demand. Pass this function to a DM_GeometriesConfiguration
     */
    static DM_PainterToOsgElements::NormalArrayType* staticGetOrCreateNormalArrayFunction(void *context);

    /**
     * @brief Static method to create the global attrib array (for points) only on demand. Pass this function to a DM_GeometriesConfiguration
     */
    static osg::Array* staticGetOrCreateAttribArrayFunction(void *context);

protected:
    void run();

public slots:
    /**
     * @brief Add a itemdrawable to the collection to convert it
     */
    void addItemDrawable(CT_AbstractItemDrawable &item, const QColor &defaultColor);

    /**
     * @brief Remove a itemdrawable from the collection
     */
    void removeItemDrawable(CT_AbstractItemDrawable &item);
    void removeItemDrawable(CT_AbstractItemDrawable *item);

private slots:
    /**
     * @brief Called when an itemdrawable is destroyed
     */
    void itemDrawableDestroyed(QObject *o);

    /**
     * @brief Called by the timer when time out and must convert the collection of elements (m_toConvert)
     */
    void computeCollection();

    void computeCollectionFinished();

signals:
    /**
     * @brief Emitted when new results is available
     */
    void newResultAvailable();

    /**
     * @brief Emitted to start the timer that call the method computeCollection()
     */
    void mustStartTimer();
};

#endif // DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H
