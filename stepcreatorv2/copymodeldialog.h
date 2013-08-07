#ifndef COPYMODELDIALOG_H
#define COPYMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "widgets/abstractcopywidget.h"
#include "models/abstractinmodel.h"
#include "models/abstractcopymodel.h"


namespace Ui {
    class COPYModelDialog;
}

class COPYModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit COPYModelDialog(QStandardItemModel *inModel, QWidget *parent = 0);
    ~COPYModelDialog();

    void init();

    static void recursiveAddChildren(AbstractCopyModel* copyModel, AbstractInModel* inModel);

    AbstractCopyModel::CopyIncludesNeeds copyIncludesNeeded();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_pb_delete_clicked();
    void on_pb_addGroup_clicked();
    void on_pb_addItem_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_buttonBox_rejected();

    void accept();

    void on_pb_cancelDelete_clicked();

private:
    Ui::COPYModelDialog *ui;
    QStandardItemModel *_model;
    QStandardItemModel *_inModel;
    QVBoxLayout *_layout;
    AbstractCopyWidget *_activeWidget;

    static void deleteItem(AbstractCopyModel *item, QStandardItemModel *model, AbstractCopyWidget *&activeWidget);
    static void cancelDelete(AbstractCopyModel *item);
};

#endif // COPYMODELDIALOG_H
