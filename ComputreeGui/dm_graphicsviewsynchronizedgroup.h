#ifndef DM_GRAPHICSVIEWSYNCHRONIZEDGROUP_H
#define DM_GRAPHICSVIEWSYNCHRONIZEDGROUP_H

#include "dm_abstractviewsynchronizedgroup.h"
#include "dm_graphicsviewsynchronizedgroupoptions.h"
#include "dm_graphicsview.h"

class DM_GraphicsViewSynchronizedGroup : public QObject, public DM_AbstractViewSynchronizedGroup
{
    Q_OBJECT

public:
    DM_GraphicsViewSynchronizedGroup();

    void setOptions(const DM_GraphicsViewSynchronizedGroupOptions &newOptions);

    virtual void addGraphicsView(DM_GraphicsView *gw);
    virtual void removeGraphicsView(DM_GraphicsView *gw);

private:

    QList<DM_GraphicsView*>                 _gwList;
    DM_GraphicsViewSynchronizedGroupOptions _options;

public slots:

    void setCameraPointOfViewForAll(const DM_GraphicsViewCamera *cam);

    void setGraphicsViewOptionsForAll(DM_GraphicsViewOptions *newOptions);

private slots:

    void syncCamera();
    void syncOptions();
};

#endif // DM_GRAPHICSVIEWSYNCHRONIZEDGROUP_H
