#ifndef GABOUTPLUGINSDIALOG_H
#define GABOUTPLUGINSDIALOG_H

#include <QDialog>
#include <QTextStream>
#include "cdm_pluginmanager.h"

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

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
    QMap<QString, CT_VirtualAbstractStep*>    _stepList;

    bool                    _mustReloadStep;

    void initView();
    void createItemsForLevelRecursively(QTreeWidgetItem *parent, const CT_MenuLevel *level, const CT_AbstractStepPlugin *plugin);
    void createItemsForStep(QTreeWidgetItem *parent, CT_VirtualAbstractStep *step);
    void createItemsForAction(QTreeWidgetItem *parent, CT_AbstractAction *act);
    void createItemsForExporter(QTreeWidgetItem *parent, CT_AbstractExporter *ex);
    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const;
    void recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const;
    void recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_AbstractModel *rModel) const;
    void exportStepData(QTextStream &stream, CT_VirtualAbstractStep *step);
    void convertHTMLtoTextile(QString &input);

private slots:

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pushButtonConfigurer_clicked();
    void on_pushButtonRecharger_clicked();
    void on_pb_redmineExport_clicked();
    void on_pb_stepInfo_clicked();
};

#endif // GABOUTPLUGINSDIALOG_H
