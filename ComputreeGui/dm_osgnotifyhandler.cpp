#include "dm_osgnotifyhandler.h"

#include "dm_guimanager.h"

DM_OSGNotifyHandler::DM_OSGNotifyHandler()
{
}

void DM_OSGNotifyHandler::notify(osg::NotifySeverity severity, const char *message)
{
    if(severity == osg::FATAL)
        GUI_LOG->addFatalMessage(LogInterface::gui, QString(message));
    else if(severity == osg::WARN)
        GUI_LOG->addWarningMessage(LogInterface::gui, QString(message));
    else if((severity == osg::NOTICE) || (severity == osg::INFO))
        GUI_LOG->addInfoMessage(LogInterface::gui, QString(message));
    else
        GUI_LOG->addDebugMessage(LogInterface::gui, QString(message));
}
