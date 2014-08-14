#include "gaboutstepdialog.h"
#include "ui_gaboutstepdialog.h"

#include "dm_guimanager.h"

GAboutStepDialog::GAboutStepDialog(Step *step, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GAboutStepDialog)
{
    ui->setupUi(this);

    initView(step);
}

GAboutStepDialog::~GAboutStepDialog()
{
    delete ui;
}

void GAboutStepDialog::initView(Step *step)
{
    ui->scrollArea->setWidgetResizable(true);

    QString pluginName = GUI_MANAGER->getPluginManager()->getPluginName(step->getPlugin());
    ui->pluginName->setText(pluginName);
    ui->stepName->setText(step->getStepName());

    QString pageName = QString("%1_Steps_%2").arg("Fr").arg(pluginName);

    ui->lb_wiki_link->setText(QString("Page wiki : <a href=\"%1/wiki/%2#%3\">%2</a>").arg(step->getPlugin()->getPluginURL()).arg(pageName).arg(step->getStepName()));

    ui->briefDescription->setText(step->getStepDescription());

    ui->detailledDescription->setText(step->getStepDetailledDescription());

    // In models
    QTreeWidgetItem *inRootItem = new QTreeWidgetItem(ui->inModels);
    inRootItem->setData(0, Qt::DisplayRole, tr("IN Models :"));

    QList<const IInResultModel*> list = step->getInResultsModel();
    QListIterator<const IInResultModel*> it(list);

    while(it.hasNext())
    {
        recursiveCreateItemsForResultModel(inRootItem, it.next());
    }

    ui->inModels->expandAll();

    // Out models
    QTreeWidgetItem *outRootItem = new QTreeWidgetItem(ui->outModels);
    outRootItem->setData(0, Qt::DisplayRole, tr("OUT Models :"));

    QList<const IOutResultModel*> listOut = step->getOutResultsModel();
    QListIterator<const IOutResultModel*> itO(listOut);

    while(itO.hasNext())
    {
        recursiveCreateItemsForResultModel(outRootItem, itO.next());
    }

    ui->outModels->expandAll();

    // Resize widgets to contents...
    int inModelsItemCount = 0;
    QTreeWidgetItemIterator iti(ui->inModels);
    while (*iti) {
      ++inModelsItemCount;
      ++iti;
    }

    int outModelsItemCount = 0;
    QTreeWidgetItemIterator ito(ui->outModels);
    while (*ito) {
      ++outModelsItemCount;
      ++ito;
    }

    int inSize = (inModelsItemCount + 1)*ui->inModels->visualItemRect(inRootItem).height();
    int outSize = (outModelsItemCount + 1)*ui->outModels->visualItemRect(outRootItem).height();

    ui->inModels->setMinimumHeight(inSize);
    ui->outModels->setMinimumHeight(outSize);

    ui->inModels->setMaximumHeight(inSize);
    ui->outModels->setMaximumHeight(outSize);
}

void GAboutStepDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const IResultModelSimple *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->resultNameDisplayable() + " (" + rModel->resultTypeDisplayable() + ")");

    QList<IItemModelSimple*> list = rModel->getRootItemModelSimple();
    QListIterator<IItemModelSimple*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForItemModel(item, it.next());
}

void GAboutStepDialog::recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const IItemModelSimple *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,rModel->itemNameDisplayable() + " (" + rModel->itemShortTypeDisplayable() + ")");

    QList<IItemModelSimple*> list = rModel->getChildrenItemModelSimple();
    QListIterator<IItemModelSimple*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForItemModel(item, it.next());
}
