#ifndef INMODELDIALOG_H
#define INMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"


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

private:
    Ui::INModelDialog *ui;
    QStandardItemModel *_model;

};

#endif // INMODELDIALOG_H
