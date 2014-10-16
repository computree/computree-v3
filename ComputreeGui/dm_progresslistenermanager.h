#ifndef DM_PROGRESSLISTENERMANAGER_H
#define DM_PROGRESSLISTENERMANAGER_H

#include "dm_iprogresslistener.h"

#include <QList>

class DM_ProgressListenerManager : public DM_IProgressListener
{
public:
    DM_ProgressListenerManager();

    /**
     * @brief Add a new listener
     */
    void addListener(const DM_IProgressListener *l);

    /**
     * @brief Remove a listener
     */
    void removeListener(const DM_IProgressListener *l);

    /**
     * @brief Set the range of progress to all listener
     */
    void setProgressRange(const int &min, const int &max);

    /**
     * @brief Set the progress to all listener
     */
    void setProgress(const int &progress);

private:

    QList<DM_IProgressListener*> m_collection;
};

#endif // DM_PROGRESSLISTENERMANAGER_H
