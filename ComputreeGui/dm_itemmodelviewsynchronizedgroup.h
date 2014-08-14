#ifndef DM_ITEMMODELVIEWSYNCHRONIZEDGROUP_H
#define DM_ITEMMODELVIEWSYNCHRONIZEDGROUP_H

#include "dm_abstractviewsynchronizedgroup.h"
#include "dm_itemmodelviewsynchronizedgroupoptions.h"
#include "dm_documentview.h"

class DM_ItemModelViewSynchronizedGroup : public QObject, public DM_AbstractViewSynchronizedGroup
{
    Q_OBJECT

public:
    DM_ItemModelViewSynchronizedGroup();

    void setOptions(const DM_ItemModelViewSynchronizedGroupOptions &newOptions);

    void setItemModelView(const DM_DocumentView *imv);

    void addDocumentView(const DM_DocumentView *dv);
    void removeDocumentView(const DM_DocumentView *dv);

    const DM_DocumentView* itemModelView() const;
    bool containsDocumentView(const DM_DocumentView *dv) const;

    const QList<DM_DocumentView*>& documentViews() const;

private:

    DM_DocumentView                             *_imv;
    QList<DM_DocumentView*>                     _dvList;
    DM_ItemModelViewSynchronizedGroupOptions    _options;

private slots:
    void slotItemDrawableSelectionChanged(ItemDrawable *item, bool selected);
    void slotItemDrawableToBeRemoved(ItemDrawable &item);
};

#endif // DM_ITEMMODELVIEWSYNCHRONIZEDGROUP_H
