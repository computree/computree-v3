#ifndef GITEMMODELVIEWSYNCCHOOSEDIALOG_H
#define GITEMMODELVIEWSYNCCHOOSEDIALOG_H

#include <QDialog>
#include <QCheckBox>

#include "dm_itemmodelviewsynchronizedgroup.h"

namespace Ui {
class GItemModelViewSyncChooseDialog;
}

class GItemModelViewSyncChooseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GItemModelViewSyncChooseDialog(QWidget *parent = 0);
    ~GItemModelViewSyncChooseDialog();
    
    bool setSyncGroup(DM_ItemModelViewSynchronizedGroup *group);

private:
    Ui::GItemModelViewSyncChooseDialog          *ui;
    DM_ItemModelViewSynchronizedGroup           *_syncGroup;
    QList< QPair<QCheckBox*, DM_DocumentView*> > _lDoc;

public slots:
    virtual void accept();
};

#endif // GITEMMODELVIEWSYNCCHOOSEDIALOG_H
