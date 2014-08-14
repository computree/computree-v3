#ifndef GABOUTPLUGINSDIALOG_H
#define GABOUTPLUGINSDIALOG_H

#include <QDialog>
#include <QTextStream>
#include "cdm_pluginmanager.h"

class QTreeWidgetItem;

namespace Ui {
class GAboutPluginsDialog;
}

class GAboutPluginsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GAboutPluginsDialog(const CDM_PluginManager &pManager,
                                 QWidget *parent = 0);
    ~GAboutPluginsDialog();
    
    bool mustReloadStepPlugins() const;

private:
    Ui::GAboutPluginsDialog *ui;

    enum ItemFrom
    {
        I_Step,
        I_Export,
        I_Action
    };

    CDM_PluginManager       *_pManager;
    QMap<QString, Step*>    _stepList;

    bool                    _mustReloadStep;

    void initView();
    void createItemsForStep(QTreeWidgetItem *parent, Step *step);
    void createItemsForAction(QTreeWidgetItem *parent, ActionInterface *act);
    void createItemsForExporter(QTreeWidgetItem *parent, IExporter *ex);
    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const IResultModelSimple *rModel) const;
    void recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const IItemModelSimple *rModel) const;
    void exportStepData(QTextStream &stream, Step *step);
    void convertHTMLtoTextile(QString &input);

private slots:

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pushButtonConfigurer_clicked();
    void on_pushButtonRecharger_clicked();
    void on_pb_redmineExport_clicked();
    void on_pb_stepInfo_clicked();
};

#endif // GABOUTPLUGINSDIALOG_H
