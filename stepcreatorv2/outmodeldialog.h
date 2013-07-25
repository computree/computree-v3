#ifndef OUTMODELDIALOG_H
#define OUTMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "widgets/abstractoutwidget.h"


namespace Ui {
    class OUTModelDialog;
}

class OUTModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OUTModelDialog(QWidget *parent = 0);
    ~OUTModelDialog();

    QString getOutIncludes();
    void getOutItemTypesIncludes(QSet<QString> &list);

    QString getOutDefines();
    QString getOutModelsDefinitions();
    QString getOutComputeBeginning(int nbOfCopyResults);
    QString getOutComputeItemsCreations();


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
    Ui::OUTModelDialog *ui;
    QStandardItemModel *_model;
    QVBoxLayout *_layout;
    AbstractOutWidget *_activeWidget;


};

#endif // OUTMODELDIALOG_H
