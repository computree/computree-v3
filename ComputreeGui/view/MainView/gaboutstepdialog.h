#ifndef GABOUTSTEPDIALOG_H
#define GABOUTSTEPDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

namespace Ui {
class GAboutStepDialog;
}

class GAboutStepDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GAboutStepDialog(CT_VirtualAbstractStep *step, QWidget *parent = 0);
    ~GAboutStepDialog();

    void initView(CT_VirtualAbstractStep *step);

private slots:
    void on_cb_ris_toggled(bool checked);

private:

    void recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_OutAbstractModel *rModel) const;
    void recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_InAbstractModel *rModel) const;

    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const;
    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const;

    void recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_OutAbstractItemModel *iModel) const;
    void recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_InAbstractItemModel *iModel) const;

    void recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_OutAbstractItemAttributeModel *iaModel) const;
    void recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_InAbstractItemAttributeModel *iaModel) const;

    void createForChildrens(QTreeWidgetItem *parent , const CT_OutAbstractModel *model) const;
    void createForChildrens(QTreeWidgetItem *parent , const CT_InAbstractModel *model) const;

private:
    Ui::GAboutStepDialog *ui;
};

#endif // GABOUTSTEPDIALOG_H
