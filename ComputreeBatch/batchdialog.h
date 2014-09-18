#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QDialog>

#include "batch.h"
#include "batchpluginmanager.h"

namespace Ui {
    class BatchDialog;
}

class BatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchDialog(QWidget *parent = 0);
    ~BatchDialog();

private:
    Ui::BatchDialog         *ui;

    QSystemTrayIcon         *_trayIcon;
    QMenu                   *_trayIconMenu;

    Batch                   *_batch;
    BatchPluginManager      _batchPluginManager;
    BatchPluginInterface    *_pluginSelected;

    void initUi();
    void createTrayIconAndMenu();
    void populatePluginsComboBox();

private slots:
    void on_comboBoxPlugins_currentIndexChanged(int index);
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonReload_clicked();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void pluginSelectedStarted();
    void pluginSelectedFinished();

    void quit();
};

#endif // BATCHDIALOG_H
