#include "gitemmodelviewsyncchoosedialog.h"
#include "ui_gitemmodelviewsyncchoosedialog.h"

#include "dm_documentmanagerview.h"
#include "gdocumentviewforitemmodel.h"

GItemModelViewSyncChooseDialog::GItemModelViewSyncChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GItemModelViewSyncChooseDialog)
{
    ui->setupUi(this);

    _syncGroup = NULL;
}

GItemModelViewSyncChooseDialog::~GItemModelViewSyncChooseDialog()
{
    delete ui;
}

bool GItemModelViewSyncChooseDialog::setSyncGroup(DM_ItemModelViewSynchronizedGroup *group)
{
    if(group->itemModelView() == NULL)
        return false;

    _syncGroup = group;

    DM_DocumentManagerView *manager = (DM_DocumentManagerView*)group->itemModelView()->getManager();

    bool ok = false;

    int nD = manager->nbDocumentView();

    for(int i=0; i<nD; ++i)
    {
        DM_DocumentView *dv = manager->getDocumentView(i);

        if((dv != group->itemModelView())
                && (dynamic_cast<GDocumentViewForItemModel*>(dv) == NULL))
        {
            QCheckBox *cb = new QCheckBox(dv->getTitle(), this);
            cb->setChecked(group->containsDocumentView(dv));

            _lDoc.append(qMakePair(cb, dv));

            ui->verticalLayout->insertWidget(0, cb);

            ok = true;
        }
    }

    return ok;
}

void GItemModelViewSyncChooseDialog::accept()
{
    QListIterator< QPair<QCheckBox*, DM_DocumentView*> > it(_lDoc);

    while(it.hasNext())
    {
        const QPair<QCheckBox*, DM_DocumentView*> &pair = it.next();

        if(pair.first->isChecked())
            _syncGroup->addDocumentView(pair.second);
        else
            _syncGroup->removeDocumentView(pair.second);
    }

    QDialog::accept();
}
