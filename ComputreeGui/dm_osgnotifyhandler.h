#ifndef DM_OSGNOTIFYHANDLER_H
#define DM_OSGNOTIFYHANDLER_H

#include <osg/Notify>

/**
 * @brief Class used to map log of osg to GUI_LOG
 */
class DM_OSGNotifyHandler : public osg::NotifyHandler
{
public:
    DM_OSGNotifyHandler();

    void notify(osg::NotifySeverity severity, const char *message) ;
};

#endif // DM_OSGNOTIFYHANDLER_H
