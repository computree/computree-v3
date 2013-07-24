#ifndef INMODELDIALOG_H
#define INMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "widgets/abstractinwidget.h"


namespace Ui {
    class INModelDialog;
}

class INModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit INModelDialog(QWidget *parent = 0);
    ~INModelDialog();

    QString getInIncludes();
    QString getInItemTypesIncludes();

    QString getInDefines();
    QString getInModelsDefinitions();
    QString getInComputeContents();


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
    Ui::INModelDialog *ui;
    QStandardItemModel *_model;
    QVBoxLayout *_layout;
    AbstractInWidget *_activeWidget;


};

#endif // INMODELDIALOG_H
