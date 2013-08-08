#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "inmodeldialog.h"
#include "outmodeldialog.h"
#include "copymodeldialog.h"
#include "parametermodeldialog.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_modelin_clicked();

    void on_modelout_clicked();

    void on_modelcopy_clicked();

    void on_parameters_clicked();

private:
    Ui::MainWindow *ui;
    INModelDialog  *_inModelDialog;
    OUTModelDialog *_outModelDialog;
    COPYModelDialog *_copyModelDialog;
    ParameterModelDialog *_paramModelDialog;


    bool createFiles(QString directory, QString stepName);
};

#endif // MAINWINDOW_H
