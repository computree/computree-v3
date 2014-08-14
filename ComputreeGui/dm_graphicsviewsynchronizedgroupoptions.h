#ifndef DM_GRAPHICSVIEWSYNCHRONIZEDGROUPOPTIONS_H
#define DM_GRAPHICSVIEWSYNCHRONIZEDGROUPOPTIONS_H

class DM_GraphicsViewSynchronizedGroupOptions
{
public:
    DM_GraphicsViewSynchronizedGroupOptions();

    void synchronizeGraphicsViewOptions(bool enable);
    void synchronizeGraphicsViewCamera(bool enable);
    void synchronizeAll(bool enable);

    bool mustSyncGraphicsViewOptions() const;
    bool mustSyncGraphicsViewCamera() const;

private:

    bool _syncGwOptions;
    bool _syncGwCamera;
};

#endif // DM_GRAPHICSVIEWSYNCHRONIZEDGROUPOPTIONS_H
