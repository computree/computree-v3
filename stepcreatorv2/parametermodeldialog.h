#ifndef PARAMETERMODELDIALOG_H
#define PARAMETERMODELDIALOG_H

#include <QDialog>

#include "qstandarditemmodel.h"
#include "qboxlayout.h"
#include "parameters/widgets/abstractparameterwidget.h"


namespace Ui {
    class ParameterModelDialog;
}

class ParameterModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParameterModelDialog(QWidget *parent = 0);
    ~ParameterModelDialog();

    inline QStandardItemModel* getModel() {return _model;}
    void closeEvent(QCloseEvent *event);

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_pb_clear_clicked();
    void on_buttonBox_rejected();
    void accept();

    void on_pb_bool_clicked();

    void on_pb_double_clicked();

    void on_pb_int_clicked();

    void on_pb_stringchoice_clicked();

    void on_pb_excludevalue_clicked();

    void on_pb_filechoice_clicked();

    void on_pb_text_clicked();

    void on_pb_empty_clicked();

    void on_pb_up_clicked();

    void on_pb_down_clicked();

private:
    Ui::ParameterModelDialog *ui;
    QStandardItemModel *_model;
    QVBoxLayout *_layout;
    AbstractParameterWidget *_activeWidget;


};

#endif // PARAMETERMODELDIALOG_H
