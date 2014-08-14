#include "dm_graphicsviewsynchronizedgroupoptions.h"

DM_GraphicsViewSynchronizedGroupOptions::DM_GraphicsViewSynchronizedGroupOptions()
{
    _syncGwOptions = true;
    _syncGwCamera = true;
}

void DM_GraphicsViewSynchronizedGroupOptions::synchronizeGraphicsViewOptions(bool enable)
{
    _syncGwOptions = enable;
}

void DM_GraphicsViewSynchronizedGroupOptions::synchronizeGraphicsViewCamera(bool enable)
{
    _syncGwCamera = enable;
}

void DM_GraphicsViewSynchronizedGroupOptions::synchronizeAll(bool enable)
{
    synchronizeGraphicsViewOptions(enable);
    synchronizeGraphicsViewCamera(enable);
}

bool DM_GraphicsViewSynchronizedGroupOptions::mustSyncGraphicsViewOptions() const
{
    return _syncGwOptions;
}

bool DM_GraphicsViewSynchronizedGroupOptions::mustSyncGraphicsViewCamera() const
{
    return _syncGwCamera;
}
