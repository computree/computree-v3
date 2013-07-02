#ifndef INMODELDIALOG_H
#define INMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "widgets/abstractwidget.h"


namespace Ui {
    class INModelDialog;
}

class INModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit INModelDialog(QWidget *parent = 0);
    ~INModelDialog();

private slots:
    void on_pb_addResult_clicked();

    void on_pb_delete_clicked();

    void on_pb_addGroup_clicked();

    void on_pb_addItem_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::INModelDialog *ui;
    QStandardItemModel *_model;
    int _idCount;
    QVBoxLayout *_layout;
    AbstractWidget *_activeWidget;


};

#endif // INMODELDIALOG_H
