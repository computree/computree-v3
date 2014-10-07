#include "gaboutstepdialog.h"
#include "ui_gaboutstepdialog.h"

#include "dm_guimanager.h"

#include "ct_abstractstepplugin.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"

GAboutStepDialog::GAboutStepDialog(CT_VirtualAbstractStep *step, QWidget *parent) :
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

void GAboutStepDialog::initView(CT_VirtualAbstractStep *step)
{
    ui->scrollArea->setWidgetResizable(true);

    QString pluginName = GUI_MANAGER->getPluginManager()->getPluginName(step->getPlugin());

    if (pluginName.left(5) == "plug_")
    {
        pluginName.remove(0, 5);
    }

    ui->pluginName->setText(pluginName);
    ui->stepName->setText(step->getStepName());

    if (step->getPlugin()->getPluginURL() == step->getStepURL())
    {
        ui->lb_wiki_link->setText(QString("Aide en ligne : <a href=\"%1\">Page internet du plugin</a>").arg(step->getPlugin()->getPluginURL()));
    } else {
        ui->lb_wiki_link->setText(QString("Aide en ligne : <a href=\"%1\">Page internet de cette étape</a>").arg(step->getStepURL()));
    }

    ui->briefDescription->setText(step->getStepDescription());

    ui->detailledDescription->setText(step->getStepDetailledDescription());

    // In models
    QTreeWidgetItem *inRootItem = new QTreeWidgetItem(ui->inModels);
    inRootItem->setData(0, Qt::DisplayRole, tr("IN Models :"));

    QList<const CT_InAbstractResultModel*> list = step->getInResultsModel();
    QListIterator<const CT_InAbstractResultModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForResultModel(inRootItem, it.next());

    ui->inModels->expandAll();

    // Out models
    QTreeWidgetItem *outRootItem = new QTreeWidgetItem(ui->outModels);
    outRootItem->setData(0, Qt::DisplayRole, tr("OUT Models :"));

    QList<const CT_OutAbstractResultModel*> listOut = step->getOutResultsModel();
    QListIterator<const CT_OutAbstractResultModel*> itO(listOut);

    while(itO.hasNext())
        recursiveCreateItemsForResultModel(outRootItem, itO.next());

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

void GAboutStepDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_OutAbstractModel *model) const
{
    const CT_OutAbstractResultModel *rModel = dynamic_cast<const CT_OutAbstractResultModel*>(model);

    if(rModel != NULL)
    {
        recursiveCreateItemsForResultModel(parent, rModel);
    }
    else
    {
        const CT_OutAbstractItemModel *iModel = dynamic_cast<const CT_OutAbstractItemModel*>(model);

        if(iModel != NULL)
        {
            recursiveCreateItemsForItemModel(parent, iModel);
        }
        else
        {
            const CT_OutAbstractItemAttributeModel *iaModel = dynamic_cast<const CT_OutAbstractItemAttributeModel*>(model);

            if(iaModel != NULL)
                recursiveCreateItemsForItemAttributesModel(parent, iaModel);
            else
                qFatal("In GAboutStepDialog::recursiveCreateItemsForModel : model is not known");
        }
    }
}

void GAboutStepDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_InAbstractModel *model) const
{
    const CT_InAbstractResultModel *rModel = dynamic_cast<const CT_InAbstractResultModel*>(model);

    if(rModel != NULL)
    {
        recursiveCreateItemsForResultModel(parent, rModel);
    }
    else
    {
        const CT_InAbstractItemModel *iModel = dynamic_cast<const CT_InAbstractItemModel*>(model);

        if(iModel != NULL)
        {
            recursiveCreateItemsForItemModel(parent, iModel);
        }
        else
        {
            const CT_InAbstractItemAttributeModel *iaModel = dynamic_cast<const CT_InAbstractItemAttributeModel*>(model);

            if(iaModel != NULL)
                recursiveCreateItemsForItemAttributesModel(parent, iaModel);
            else
                qFatal("In GAboutStepDialog::recursiveCreateItemsForModel : model is not known");
        }
    }
}

void GAboutStepDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->resultName());

    createForChildrens(item, rModel);
}

void GAboutStepDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->displayableName());

    createForChildrens(item, rModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_OutAbstractItemModel *iModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iModel->displayableName() + " (" + iModel->itemDrawable()->name() + ")");

    createForChildrens(item, iModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_InAbstractItemModel *iModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iModel->displayableName() + " (" + iModel->itemShortType() + ")");

    createForChildrens(item, iModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_OutAbstractItemAttributeModel *iaModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iaModel->itemAttribute()->displayableName() + " (" + iaModel->itemAttribute()->typeToString() + ")");

    createForChildrens(item, iaModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_InAbstractItemAttributeModel *iaModel) const
{
    const CT_AbstractCategory* category = CT_Context::staticInstance()->categoryManager()->findByUniqueName(CT_AbstractCategory::DATA_VALUE);
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iaModel->displayableName() + " (" + category->valueTypeToString(iaModel->valueType()) + ")");

    createForChildrens(item, iaModel);
}

void GAboutStepDialog::createForChildrens(QTreeWidgetItem *parent, const CT_OutAbstractModel *model) const
{
    QList<CT_AbstractModel*> list = model->childrens();
    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForModel(parent, (CT_OutAbstractModel*)it.next());
}

void GAboutStepDialog::createForChildrens(QTreeWidgetItem *parent, const CT_InAbstractModel *model) const
{
    QList<CT_AbstractModel*> list = model->childrens();
    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForModel(parent, (CT_InAbstractModel*)it.next());
}
