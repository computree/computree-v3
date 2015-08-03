#ifndef CT_ELEMENTLISTDIALOG_H
#define CT_ELEMENTLISTDIALOG_H

#include "ct_filter/abstract/ct_abstractconfigurableelement.h"
#include "pluginShared_global.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class CT_ElementListDialog;
}

class PLUGINSHAREDSHARED_EXPORT CT_ElementListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CT_ElementListDialog(QList<CT_AbstractConfigurableElement *> &elements, QWidget *parent = 0);
    ~CT_ElementListDialog();

    CT_AbstractConfigurableElement* getSelectedElement();

private:
    Ui::CT_ElementListDialog *ui;

    QMultiMap<QListWidgetItem*, CT_AbstractConfigurableElement *> _elements;
};

#endif // CT_ELEMENTLISTDIALOG_H
