#ifndef GABOUTSTEPDIALOG_H
#define GABOUTSTEPDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "interfaces.h"

namespace Ui {
class GAboutStepDialog;
}

class GAboutStepDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GAboutStepDialog(Step *step, QWidget *parent = 0);
    ~GAboutStepDialog();

    void initView(Step *step);
    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const IResultModelSimple *rModel) const;
    void recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const IItemModelSimple *rModel) const;


private:
    Ui::GAboutStepDialog *ui;
};

#endif // GABOUTSTEPDIALOG_H
