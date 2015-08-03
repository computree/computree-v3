#ifndef CT_MANAGECONFIGURABLEELEMENTSDIALOG_H
#define CT_MANAGECONFIGURABLEELEMENTSDIALOG_H

#include "ct_filter/abstract/ct_abstractconfigurableelement.h"
#include "ct_view/tools/ct_elementlistdialog.h"
#include "pluginShared_global.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class CT_ManageConfigurableElementsDialog;
}

class PLUGINSHAREDSHARED_EXPORT CT_ManageConfigurableElementsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CT_ManageConfigurableElementsDialog(QList<CT_AbstractConfigurableElement*> &elements, QString title, QWidget *parent = 0);
    ~CT_ManageConfigurableElementsDialog();

private slots:
    void on_pb_new_clicked();

    void on_pb_modify_clicked();

    void on_pb_delete_clicked();

private:
    Ui::CT_ManageConfigurableElementsDialog *ui;

    CT_ElementListDialog*   _listDialog;

    QList<CT_AbstractConfigurableElement*> _baseElements;
    QMap<QListWidgetItem*, CT_AbstractConfigurableElement*> _addedElements;
};

#endif // CT_MANAGECONFIGURABLEELEMENTSDIALOG_H
