#ifndef DM_ABSTRACTWORKER_H
#define DM_ABSTRACTWORKER_H

#include <QString>
#include <QObject>
#include <QPair>

/**
 * @brief A Worker can be used with a QThread to do a process in a separate thread than the gui thread :
 *
 * QThread* thread = new QThread;
 * DM_AbstractWorker* worker = new ....Worker();
 * worker->moveToThread(thread);
 *
 * DM_AbstractWorker::staticConnectWorkerToThread(worker);
 *
 * thread->start();
 */
class DM_AbstractWorker : public QObject
{
    Q_OBJECT

public:
    DM_AbstractWorker();
    virtual ~DM_AbstractWorker();

    /**
     * @brief Init the worker before start the thread !
     */
    virtual void init();

    /**
     * @brief return true if the worker is canceled
     */
    bool isCanceled() const;

    /**
     * @brief return true if the worker is finished
     */
    bool isFinished() const;

    /**
     * @brief Return true if it was a error
     */
    bool isAnError() const;

    /**
     * @brief Return the error
     */
    QString error() const;

    /**
     * @brief return the progress
     */
    int progress() const;

    /**
     * @brief Add a unknown data (if you want to backup something or if you want than the worker manager the deletion
     *        of a data when it will be destroyed)
     */
    void addData(QObject *data, bool deleteInDestructor = false);

    /**
     * @brief Static method to connect a worker to his thread
     */
    static void staticConnectWorkerToThread(DM_AbstractWorker *worker, bool deleteWorkerWhenFinished = false, bool deleteThreadWhenFinished = false, bool initWorker = true);

protected slots:
    virtual void setProgress(const int &progress);
    virtual void setProgressRanged(const int &min, const int &max);
    void setError(const QString &err);
    void setFinished();

private:
    int                             m_progress;
    int                             m_minProgress;
    int                             m_rangeProgress;
    bool                            m_rangeProgressSpecial;
    bool                            m_cancel;
    bool                            m_finished;
    QString                         m_error;
    QList< QPair<QObject*, bool> >  m_data;

public slots:

    /**
     * @brief apply the process
     */
    virtual void apply() = 0;

    /**
     * @brief cancel the process
     */
    void cancel();

signals:
    void progressChanged(const int &progress);
    void canceled();
    void finished(bool canceled = false);
    void error(const QString &err);
};

#endif // DM_ABSTRACTWORKER_H
