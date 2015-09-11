#ifndef DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H
#define DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H

#include <QThread>
#include <QQueue>
#include <QMutex>

#include "view/DocumentView/GraphicsViews/3D/Converter/dm_singleitemdrawabletoosgworker.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_geometriesconfiguration.h"
#include "view/DocumentView/gdocumentviewforgraphics.h"

/**
 * @brief This class will convert all itemdrawable passed in parameter to osg element if you want to add it in
 *        a scene. The conversion is made in multithread (async) !
 */
class DM_MultipleItemDrawableToOsgWorker : public QThread
{
    Q_OBJECT

public:
    DM_MultipleItemDrawableToOsgWorker(GOsgGraphicsView &view);
    ~DM_MultipleItemDrawableToOsgWorker();

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

private:
    GOsgGraphicsView                                                        &m_view;
    QQueue<CT_AbstractItemDrawable*>                                        m_queue;
    QHash<CT_AbstractItemDrawable*, QColor>                                 m_infos;

    DM_GeometriesConfiguration                                              m_geometriesConfiguration;

    QMutex                                                                  *m_mutex;
    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult >        m_results;
    bool                                                                    m_stop;

    QTimer                                                                  m_timerComputeQueue;

    void createGlobalShader();
    void createLocalShader();
    void createGeometriesConfiguration();

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
     * @brief Called when the worker is finished
     */
    void workerFinished(DM_SingleItemDrawableToOsgWorker *w);

    /**
     * @brief Called by the timer when time out
     */
    void computeQueue();
signals:
    /**
     * @brief Emitted when new results is available
     */
    void newResultAvailable();

    void mustStartTimer();
};

#endif // DM_MULTIPLEITEMDRAWABLETOOSGWORKER_H
