#include "dm_abstractworker.h"

#include <QThread>

DM_AbstractWorker::DM_AbstractWorker()
{
}

DM_AbstractWorker::~DM_AbstractWorker()
{
    while(!m_data.isEmpty())
    {
        QPair<QObject*, bool> pair = m_data.takeFirst();

        if(pair.second)
            pair.first->deleteLater();
    }
}

void DM_AbstractWorker::init()
{
    m_cancel = false;
    m_finished = false;
    m_error = "";
    m_progress = 0;
    setProgressRanged(0, 100);
}

bool DM_AbstractWorker::isCanceled() const
{
    return m_cancel;
}

bool DM_AbstractWorker::isFinished() const
{
    return m_finished;
}

bool DM_AbstractWorker::isAnError() const
{
    return !m_error.isEmpty();
}

QString DM_AbstractWorker::error() const
{
    return m_error;
}

int DM_AbstractWorker::progress() const
{
    return m_progress;
}

void DM_AbstractWorker::addData(QObject *data, bool deleteInDestructor)
{
    m_data.append(qMakePair(data, deleteInDestructor));
}

void DM_AbstractWorker::staticConnectWorkerToThread(DM_AbstractWorker *worker, bool deleteWorkerWhenFinished, bool deleteThreadWhenFinished, bool initWorker)
{
    QThread *thread = worker->thread();

    QObject::connect(thread, SIGNAL(started()), worker, SLOT(apply()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));

    if(deleteWorkerWhenFinished)
        QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    if(deleteThreadWhenFinished)
        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    if(initWorker)
        worker->init();
}

void DM_AbstractWorker::setProgress(int progress)
{
    int realProgress = progress;

    if(m_rangeProgressSpecial)
        realProgress = ((progress-m_minProgress)*100 / m_rangeProgress);

    if(m_progress != realProgress)
    {
        if((realProgress % 5) == 0) {
            m_progress = realProgress;

            emit progressChanged(m_progress);
        }
    }
}

void DM_AbstractWorker::setProgressRanged(const int &min, const int &max)
{
    m_minProgress = min;
    m_rangeProgress = max-min;

    m_rangeProgressSpecial = ((m_minProgress != 0) || (m_rangeProgress != 100));
}

void DM_AbstractWorker::setError(const QString &err)
{
    m_error = err;

    if(isAnError())
        emit error(m_error);
}

void DM_AbstractWorker::setFinished()
{
    m_finished = true;

    emit finished(isCanceled());
}

void DM_AbstractWorker::cancel()
{
    m_cancel = true;

    emit canceled();
}
