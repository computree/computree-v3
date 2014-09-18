#include "gstepmanager.h"

#include "cdm_configfile.h"

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>

GStepManager::GStepManager(QWidget *parent) : QWidget(parent)
{

}

void GStepManager::init(CDM_StepManager &stepManager)
{
    _stepManager = &stepManager;

    _delegate = new MyTreeDelegate(&_view);

    _view.setModel(&_model);
    _view.setItemDelegate(_delegate);

    QStringList header;
    header << tr("Nom");
    header << tr("Progression");
    header << tr("Temps");

    _model.setHorizontalHeaderLabels(header);

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(&_view);

    connect(_stepManager, SIGNAL(stepAdded(CT_VirtualAbstractStep*)), this, SLOT(stepAdded(CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(_stepManager, SIGNAL(stepToBeRemoved(CT_VirtualAbstractStep*)), this, SLOT(stepToBeRemoved(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(this, SIGNAL(addResult(QStandardItem*,CT_AbstractResult*)), this, SLOT(resultToAdd(QStandardItem*,CT_AbstractResult*)), Qt::QueuedConnection);
    connect(this, SIGNAL(removeItem(QStandardItem*)), this, SLOT(itemToRemove(QStandardItem*)), Qt::QueuedConnection);
}

////////////////// PRIVATE /////////////////

QList<QStandardItem *> GStepManager::createItemsForStep(CT_VirtualAbstractStep &step)
{
    QList<QStandardItem *> list;

    // nom de l'etape
    MyQStandardItem *item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepName, getStepName(step));

    item->setToolTip(step.getToolTip());

    item->setEditable(false);
    list.append(item);

    // progression
    item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepProgress, step.getProgress());
    connect(&step, SIGNAL(inProgress(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    // temps coul
    item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepElapsedTime, step.getExecuteTime());
    item->setEditable(false);
    connect(&step, SIGNAL(elapsed(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    setStepItemBackgroundColor(step, list);

    return list;
}

QList<QStandardItem *> GStepManager::createItemsForResult(CT_AbstractResult &res)
{
    QList<QStandardItem *> list;

    // nom du rsultat
    MyQStandardItem *item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultName, res.getName());
    item->setEditable(false);
    list.append(item);

    // dchargement de la mmoire
    item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultProgress, res.getClearFromMemoryProgress());
    item->setEditable(false);
    connect(&res, SIGNAL(clearFromMemoryInProgress(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    connect(&res, SIGNAL(serializeInProgress(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    // visibilit
    item = new MyQStandardItem(NULL, &res, MyQStandardItem::Empty, QString(""));
    item->setEditable(false);
    list.append(item);

    setResultItemBackgroundColor(ResultInfo(res.getClearFromMemoryProgress() < 100, 0), list);

    return list;
}

void GStepManager::setStepItemBackgroundColor(CT_VirtualAbstractStep &step, QList<QStandardItem *> &list)
{
    QListIterator<QStandardItem*> it(list);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        item->setData(QColor(170, 186, 225), Qt::BackgroundColorRole);
    }
}

void GStepManager::setResultItemBackgroundColor(ResultInfo info, QList<QStandardItem *> &list)
{
    QListIterator<QStandardItem*> it(list);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        if(info._isClearedFromMemory)
        {
            item->setData(QColor(236, 208, 209), Qt::BackgroundColorRole);
        }
        else if(info._isSerialized)
        {
            item->setData(QColor(206, 238, 211), Qt::BackgroundColorRole);
        }
        else
        {
            item->setData(QColor(220, 220, 220), Qt::BackgroundColorRole);
        }
    }
}

MyQStandardItem* GStepManager::findItem(CT_VirtualAbstractStep *step)
{
    MyQStandardItem *item = getSelectedItem();

    if((item != NULL)
        && (item->step() == step))
    {
        return item;
    }

    return recursiveFindItem(step);
}

MyQStandardItem* GStepManager::findItem(CT_AbstractResult *res)
{
    MyQStandardItem *item = getSelectedItem();

    if((item != NULL)
        && (item->result() == res))
    {
        return item;
    }

    return recursiveFindItem(res);
}

MyQStandardItem* GStepManager::getSelectedItem()
{
    QModelIndexList list = _view.selectionModel()->selectedRows();

    if(!list.isEmpty())
    {
        return (MyQStandardItem*)_model.itemFromIndex(list.first());
    }

    return NULL;
}

MyQStandardItem* GStepManager::recursiveFindItem(CT_VirtualAbstractStep *step)
{
    CT_VirtualAbstractStep* parent = step->parentStep();
    QStandardItem *parentItem = NULL;

    if(parent != NULL)
    {
        // on recherche le parent
        parentItem = recursiveFindItem(parent);
    }
    // on est arriver au dbut de l'arbre
    else
    {
        // on recherche l'tape dans les enfants
        // de l'item root
        parentItem = _model.invisibleRootItem();
    }

    if(parentItem != NULL)
    {
        int n = parentItem->rowCount();

        for(int i=0; i<n; ++i)
        {
            MyQStandardItem *item = (MyQStandardItem*)parentItem->child(i);

            if(item->step() == step)
            {
                return item;
            }
        }
    }

    return NULL;
}

MyQStandardItem* GStepManager::recursiveFindItem(CT_AbstractResult *res)
{
    CT_VirtualAbstractStep* parent = res->parentStep();

    MyQStandardItem *stepItem = recursiveFindItem(parent);

    if(stepItem != NULL)
    {
        return getItemForResult(stepItem, res);
    }

    return NULL;
}

MyQStandardItem* GStepManager::getItemForResult(QStandardItem *stepItem, CT_AbstractResult *res)
{
    int n = stepItem->rowCount();

    for(int i=0; i<n; ++i)
    {
        MyQStandardItem *item = (MyQStandardItem*)stepItem->child(i);

        if(item->result() == res)
        {
            return item;
        }
    }

    return NULL;
}

QList<MyQStandardItem *> GStepManager::getItemsForResult(QStandardItem *stepItem, CT_AbstractResult *res)
{
    QList<MyQStandardItem *> list;

    int n = stepItem->rowCount();

    for(int i=0; i<n; ++i)
    {
        MyQStandardItem *item = (MyQStandardItem*)stepItem->child(i);

        if(item->result() == res)
        {
            int nc = _model.columnCount();

            for(int j=0; j<nc; ++j)
            {
                item = (MyQStandardItem*)stepItem->child(i, j);
                list.append(item);
            }

            return list;
        }
    }

    return list;
}

QString GStepManager::getStepName(CT_VirtualAbstractStep &step)
{
    CT_AbstractStepLoadFile *stepLF = dynamic_cast<CT_AbstractStepLoadFile*>(&step);

    if(stepLF != NULL)
    {
        QString filePath = stepLF->getFilePath();
        QFileInfo info(filePath);

        return QString("%1%2").arg(step.isSettingsModified() ? "*" : "").arg(info.fileName());
    }

    return QString("%1%2").arg(step.isSettingsModified() ? "*" : "").arg(step.getStepName());
}

////////////// PRIVATE SLOTS //////////////

void GStepManager::stepAdded(CT_VirtualAbstractStep *step)
{
    CT_VirtualAbstractStep *parentStep = step->parentStep();
    QStandardItem *item = NULL;

    if(parentStep == NULL)
    {
        item = _model.invisibleRootItem();
    }
    else
    {
        item = findItem(parentStep);
    }

    if(item != NULL)
    {
        connect(step, SIGNAL(resultAdded(CT_AbstractResult*)), this, SLOT(resultAdded(CT_AbstractResult*)), Qt::DirectConnection);

        QList<QStandardItem *> newItems = createItemsForStep(*step);

        item->appendRow(newItems);

        _view.expand(_model.indexFromItem(newItems.first()));
    }
}

void GStepManager::stepToBeRemoved(CT_VirtualAbstractStep *step)
{
    QStandardItem *item = findItem(step);

    if(item != NULL)
    {
        // on emet un signal pour qu'on supprime
        // l'item dans une boucle d'vnement. Que se passerai-t-il
        // si on ne faisait pas a : lorsqu'un thread supprime une tape
        // du step manager et qu'on reoit directement le signal,
        // on n'est pas dans la boucle d'vnement donc la vue est peut tre
        // en train d'tre mis  jour, donc si on supprime l'item pendant la
        // mise  jour du qteeview il peut y avoir un plantage.
        emit removeItem(item);
    }
}

void GStepManager::resultAdded(CT_AbstractResult *res)
{
    QStandardItem *item = findItem(res->parentStep());

    if(item != NULL)
    {
        _mutexResList.lock();
        _resToBeAddedList.append(res);
        _mutexResList.unlock();

        // on emet un signal pour les mmes raisons que
        // lors de la suppression d'tape
        emit addResult(item, res);
    }
}

void GStepManager::resultToAdd(QStandardItem *parentItem, CT_AbstractResult *res)
{
    _mutexResList.lock();

    int index;

    // si le rsultat est toujours dans la liste.
    if((index = _resToBeAddedList.indexOf(res)) >= 0)
    {
        parentItem->insertRow(0, createItemsForResult(*res));
        _resToBeAddedList.removeAt(index);
    }

    _mutexResList.unlock();
}

void GStepManager::itemToRemove(QStandardItem *item)
{
    if(item->parent() != NULL)
    {
        item->parent()->removeRow(item->row());
    }
    else
    {
        _model.invisibleRootItem()->removeRow(item->row());
    }
}
