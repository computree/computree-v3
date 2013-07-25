#ifndef COPYMODELDIALOG_H
#define COPYMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "widgets/abstractcopywidget.h"


namespace Ui {
    class COPYModelDialog;
}

class COPYModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit COPYModelDialog(QWidget *parent = 0);
    ~COPYModelDialog();

    QString getCopyIncludes();
    void getCopyItemTypesIncludes(QSet<QString> &list);

    QString getCopyDefines();
    QString getCopyModelsDefinitions();
    QString getCopyComputeBeginning(int nbOfCopyResults);
    QString getCopyComputeItemsCreations();


private slots:
    void on_pb_addResult_clicked();
    void on_pb_delete_clicked();
    void on_pb_addGroup_clicked();
    void on_pb_addItem_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_pb_clear_clicked();
    void on_buttonBox_rejected();

    void accept();

private:
    Ui::COPYModelDialog *ui;
    QStandardItemModel *_model;
    QVBoxLayout *_layout;
    AbstractCopyWidget *_activeWidget;


};

#endif // COPYMODELDIALOG_H
