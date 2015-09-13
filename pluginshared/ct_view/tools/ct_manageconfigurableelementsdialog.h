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
    explicit CT_ManageConfigurableElementsDialog(QString title, QList<CT_AbstractConfigurableElement*> &elements, QList<CT_AbstractConfigurableElement*> *selectedElements, QWidget *parent = 0);
    ~CT_ManageConfigurableElementsDialog();

    QString getConfig() const;
    QString setConfig(const QString &config);

     void setSuffix(QString suffixe);
     QString getSuffix();

private slots:
    void on_pb_new_clicked();
    void on_pb_modify_clicked();
    void on_pb_delete_clicked();

    void cancelClicked();
    void trimLE(QString str);

private:
    Ui::CT_ManageConfigurableElementsDialog *ui;

    CT_ElementListDialog*   _listDialog;

    QList<CT_AbstractConfigurableElement*> _baseElements;
    QList<CT_AbstractConfigurableElement*> *_selectedElements;
    QMap<QListWidgetItem*, CT_AbstractConfigurableElement*> _addedElements;
};

#endif // CT_MANAGECONFIGURABLEELEMENTSDIALOG_H
