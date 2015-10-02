#include "ctg_inresultmodelpossibilities.h"
#include "ui_ctg_inresultmodelpossibilities.h"

#include "ct_inresultmodelmanager.h"
#include "ct_outabstractresultmodel.h"

#include "ct_virtualabstractstep.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_view/inModel/ctg_inturnmanager.h"

#include <QMimeData>
#include <QModelIndex>
#include <QMessageBox>

#define HELP_COLUMN 2

QMimeData* CTG_InResultModelPossibilitiesDragModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = QStandardItemModel::mimeData(indexes);

    if(!indexes.isEmpty())
        mimeData->setText(indexes.first().data(Qt::UserRole+1).toString());

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
    ui->treeView->setDragEnabled(false);

    // to have text with black color when it was disabled
    ui->treeView->setStyleSheet(QString("QTreeView::item {"
                                "color: rgb(0, 0, 0);"
                                "}"
                                "QTreeView::item:has-children:!hover:!selected {"
                                "background: rgb(172, 226, 169);"
                                "}"
                                "QTreeView::item:!has-children:!hover:!selected {"
                                "background: rgb(220, 220, 220);"
                                "}"));
    m_readOnly = false;

    _manager = NULL;
    m_gTurnManager = NULL;

    constructHeader();
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

void CTG_InResultModelPossibilities::setInTurnManager(const CTG_InTurnManager *manager)
{
    m_gTurnManager = (CTG_InTurnManager*)manager;
}

void CTG_InResultModelPossibilities::setReadOnly(bool enabled)
{
    m_readOnly = enabled;
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
    header << tr("Etape");
    header << tr("Aide"); // if you change the header don't missing to change HELP_COLUMN defined at the top of this file

    _viewModel.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::Fixed);
#else
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::Fixed);
#endif
}

void CTG_InResultModelPossibilities::updateModel()
{
    int n = _viewModel.invisibleRootItem()->rowCount();

    for(int i=0; i<n; ++i) {
        QStandardItem *item = _viewModel.invisibleRootItem()->child(i, 0);

        int m = item->rowCount();

        for(int j=0; j<m; ++j) {
            QStandardItem *pItem = item->child(j, 0);

            // TODO WARNING : this will be correct if we have only ONE turn !
            CT_InStdModelPossibility *possibilityOfCurrentTurn = NULL;

            if(m_gTurnManager != NULL)
                possibilityOfCurrentTurn = m_gTurnManager->getPossibilityFromMimeDataForCurrentTurn(pItem->data().toString(), NULL);

            // Nom du modèle de sortie (avec case à cocher)
            pItem->setCheckState(((possibilityOfCurrentTurn != NULL) && possibilityOfCurrentTurn->isSelected()) ? Qt::Checked : Qt::Unchecked);
            // modifiable ou non en fonction du paramètre readonly
            pItem->setEnabled(!m_readOnly);
        }
    }
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
    rootName->setData(resModel->description());
    retList.append(rootName);

    // Etape du modèle d'entrée
    /*rootOther = new QStandardItem(resModel->step()->getStepCustomName() == resModel->step()->getStepDisplayableName() ? resModel->step()->getStepExtendedDisplayableName() : resModel->step()->getStepCustomName());
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    retList.append(rootOther);*/
    QStandardItem *rootOther = new QStandardItem("");
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    retList.append(rootOther);

    // Aide
    rootOther = new QStandardItem("");
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    rootOther->setIcon(QIcon(":/Icones/Icones/help.png"));
    retList.append(rootOther);

    int i = 0;

    QListIterator<CT_InStdModelPossibility*> it(possibilities);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        // drag data = nom du modèle d'entrée ;; index de la possibilité
        QString dragText = QString("%1;;%2").arg(resModel->uniqueName()).arg(i);

        QList<QStandardItem*> rowList;

        // Nom du modèle de sortie (avec case à cocher)
        QStandardItem *item = new QStandardItem(possibility->outModel()->displayableName());
        item->setDragEnabled(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        item->setEnabled(!m_readOnly);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        // Etape du modèle de sortie
        item = new QStandardItem(possibility->outModel()->step()->getStepCustomName() == possibility->outModel()->step()->getStepDisplayableName() ? possibility->outModel()->step()->getStepExtendedDisplayableName() : possibility->outModel()->step()->getStepCustomName());
        item->setDragEnabled(false);
        item->setEditable(false);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        // Vide
        item = new QStandardItem("");
        item->setDragEnabled(false);
        item->setEditable(false);
        item->setData(dragText, Qt::UserRole+1);
        rowList.append(item);

        rootName->appendRow(rowList);

        ++i;
    }

    return retList;
}

void CTG_InResultModelPossibilities::showEvent(QShowEvent *event)
{
    updateModel();

    QWidget::showEvent(event);
}

void CTG_InResultModelPossibilities::on_treeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = _viewModel.itemFromIndex(index);
    QStandardItem *parent = _viewModel.itemFromIndex(index.parent());

    if(parent != NULL)
        item = parent->child(item->row(), 0);
    else
        item = _viewModel.invisibleRootItem()->child(item->row(), 0);

    if((item != NULL) && (item->rowCount() != 0) && (index.column() == HELP_COLUMN)) {
        QMessageBox::information(this, tr("Aide"), tr("<html>Description du résultat d'entrée : <br/><br/>%1</html>").arg(item->data().toString()));
        return;
    }

    bool ok = false;

    if(item != NULL) {
        if(item->data().isValid()) {
            ok = true;
            if(item->checkState() == Qt::Checked) {
                emit enableResultPossibility(item->data().toString());
            } else {
                emit disableResultPossibility(item->data().toString());
            }

            updateModel();

            ui->treeView->selectionModel()->select(index, QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
        }
    }

    if(!ok)
        emit showResultPossibility("");
}

void CTG_InResultModelPossibilities::on_treeView_doubleClicked(const QModelIndex &index)
{
    bool ok = false;

    QStandardItem *item = _viewModel.itemFromIndex(index);

    if(item->parent() != NULL) {
        item = item->parent()->child(item->row(), 0);

        if(item->data().isValid()){
            if(!m_readOnly) {
                if(item->checkState() == Qt::Checked)
                    item->setCheckState(Qt::Unchecked);
                else
                    item->setCheckState(Qt::Checked);

                if(item->checkState() == Qt::Checked) {
                    ok = true;
                    emit enableResultPossibility(item->data().toString());
                } else if(item->checkState() == Qt::Unchecked) {
                    ok = true;
                    emit disableResultPossibility(item->data().toString());
                }
            } else {
                ok = true;
                emit showResultPossibility(item->data().toString());
            }
        }
    }

    if(!ok)
        emit showResultPossibility("");
}
