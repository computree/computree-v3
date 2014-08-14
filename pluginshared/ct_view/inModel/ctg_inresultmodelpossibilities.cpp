#include "ctg_inresultmodelpossibilities.h"
#include "ui_ctg_inresultmodelpossibilities.h"

#include "ct_inresultmodelmanager.h"
#include "ct_outabstractresultmodel.h"

#include "ct_virtualabstractstep.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include <QMimeData>
#include <QModelIndex>

QMimeData* CTG_InResultModelPossibilitiesDragModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = QStandardItemModel::mimeData(indexes);

    if(!indexes.isEmpty())
    {
        mimeData->setText(indexes.first().data(Qt::UserRole+1).toString());
    }

    return mimeData;
}

CTG_InResultModelPossibilities::CTG_InResultModelPossibilities(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTG_InResultModelPossibilities)
{
    ui->setupUi(this);

    ui->treeView->setModel(&_viewModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->treeView->setDragEnabled(true);

    constructHeader();

    _manager = NULL;
}

CTG_InResultModelPossibilities::~CTG_InResultModelPossibilities()
{
    delete ui;
}

void CTG_InResultModelPossibilities::setInResultModelManager(const CT_InResultModelManager *manager)
{
    _manager = (CT_InResultModelManager*)manager;

    constructModel();

    ui->treeView->expandAll();
}

void CTG_InResultModelPossibilities::setReadOnly(bool enabled)
{
    ui->treeView->setDragEnabled(!enabled);
}

void CTG_InResultModelPossibilities::clearModel()
{
    _viewModel.invisibleRootItem()->removeRows(0, _viewModel.invisibleRootItem()->rowCount());
}

void CTG_InResultModelPossibilities::constructModel()
{
    clearModel();

    if(_manager != NULL)
    {
        QListIterator<CT_InAbstractResultModel*> it(_manager->models());

        while(it.hasNext())
        {
            QList<QStandardItem*> items = createItemsForResultModel(it.next());

            if(!items.isEmpty())
                _viewModel.invisibleRootItem()->appendRow(items);
        }
    }
}

void CTG_InResultModelPossibilities::constructHeader()
{
    QStringList header;
    header << tr("Nom des résultats");
    header << tr("Description");
    header << tr("Etape");

    _viewModel.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
#else
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
#endif
}

QList<QStandardItem*> CTG_InResultModelPossibilities::createItemsForResultModel(CT_InAbstractResultModel *resModel)
{
    QList<QStandardItem*> retList;

    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSaved();

    if(possibilities.isEmpty())
        return retList;

    // Nom du modèle d'entrée
    QStandardItem *rootName = new QStandardItem(resModel->displayableName());
    rootName->setDragEnabled(false);
    rootName->setEditable(false);
    retList.append(rootName);

    // Description du modèle d'entrée
    QStandardItem *rootOther = new QStandardItem(resModel->description());
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    retList.append(rootOther);

    // Etape du modèle d'entrée
    /*rootOther = new QStandardItem(resModel->step()->getStepCustomName() == resModel->step()->getStepName() ? resModel->step()->getStepExtendedName() : resModel->step()->getStepCustomName());
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    retList.append(rootOther);*/

    int i = 0;

    QListIterator<CT_InStdModelPossibility*> it(possibilities);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        // drag data = nom du modèle d'entrée ;; index de la possibilité
        QString dragText = QString("%1;;%2").arg(resModel->uniqueName()).arg(i);

        QList<QStandardItem*> rowList;

        // Nom du modèle de sortie
        QStandardItem *item = new QStandardItem(possibility->outModel()->displayableName());
        item->setDragEnabled(true);
        item->setEditable(false);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        // Description du modèle de sortie
        item = new QStandardItem(possibility->outModel()->description());
        item->setDragEnabled(true);
        item->setEditable(false);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        // Etape du modèle de sortie
        item = new QStandardItem(possibility->outModel()->step()->getStepCustomName() == possibility->outModel()->step()->getStepName() ? possibility->outModel()->step()->getStepExtendedName() : possibility->outModel()->step()->getStepCustomName());
        item->setDragEnabled(true);
        item->setEditable(false);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        rootName->appendRow(rowList);

        ++i;
    }

    return retList;
}

void CTG_InResultModelPossibilities::on_treeView_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = _viewModel.itemFromIndex(index);

    if(item->data().isValid())
        emit resultChoosed(item->data().toString());
}
