#include "gaboutmemory.h"
#include "ui_gaboutmemory.h"

#include "ct_global/ct_context.h"
#include "ct_tools/repository/ct_repositoryaccess.h"
#include "ct_cloud/tools/ct_globalpointcloudmanager.h"
#include "ct_cloud/tools/ct_globaledgecloudmanager.h"
#include "ct_cloud/tools/ct_globalfacecloudmanager.h"
#include "ct_cloudindex/tools/ct_cloudindexregistrationmanagert.h"

GAboutMemory::GAboutMemory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GAboutMemory)
{
    ui->setupUi(this);
}

GAboutMemory::~GAboutMemory()
{
    delete ui;
}

template<typename A, typename R>
void addToList(const QList< A* > &in, QList< R* > &out)
{
    QListIterator<A*> it(in);

    while(it.hasNext())
        out.append(it.next());
}

QString size_human(size_t size)
{
    float num = size;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}

void GAboutMemory::updateUI()
{
    ui->tableWidgetMemoryUsed->clear();
    ui->tableWidgetMemoryUsed->setColumnCount(3);
    ui->tableWidgetMemoryUsed->setHorizontalHeaderLabels(QStringList() << tr("Type") << tr("Taille") << tr("Mémoire utilisée"));
    ui->tableWidgetMemoryUsed->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    CT_RepositoryAccess access(PS_REPOSITORY);

    int row = 0;
    size_t totalMemUsed = 0;

    setRowForCloud(tr("Nuage global de points"), row++, access.globalPointCloudManager()->globalCloud(), totalMemUsed);
    setRowForCloud(tr("Nuage global de faces"), row++, access.globalFaceCloudManager()->globalCloud(), totalMemUsed);
    setRowForCloud(tr("Nuage global d'arêtes"), row++, access.globalEdgeCloudManager()->globalCloud(), totalMemUsed);

    QList<CT_AbstractCloudIndexRegistered*> l;
    addToList<CT_AbstractModifiableCloudIndexRegisteredT<CT_PointData>, CT_AbstractCloudIndexRegistered>(access.globalPointCloudIndexRegistrationManager()->modifiableCloudIndexRegistered(),
                                                                                                         l);
    addToList<CT_AbstractNotModifiableCloudIndexRegisteredT<CT_PointData>, CT_AbstractCloudIndexRegistered>(access.globalPointCloudIndexRegistrationManager()->notModifiableCloudIndexRegistered(),
                                                                                                         l);
    setRowForCloudIndexRegistered(tr("Index des points"), row++, l, totalMemUsed);

    l.clear();
    addToList<CT_AbstractModifiableCloudIndexRegisteredT<CT_Face>, CT_AbstractCloudIndexRegistered>(access.globalFaceCloudIndexRegistrationManager()->modifiableCloudIndexRegistered(),
                                                                                                         l);
    addToList<CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face>, CT_AbstractCloudIndexRegistered>(access.globalFaceCloudIndexRegistrationManager()->notModifiableCloudIndexRegistered(),
                                                                                                         l);
    setRowForCloudIndexRegistered(tr("Index des faces"), row++, l, totalMemUsed);

    l.clear();
    addToList<CT_AbstractModifiableCloudIndexRegisteredT<CT_Edge>, CT_AbstractCloudIndexRegistered>(access.globalEdgeCloudIndexRegistrationManager()->modifiableCloudIndexRegistered(),
                                                                                                         l);
    addToList<CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge>, CT_AbstractCloudIndexRegistered>(access.globalEdgeCloudIndexRegistrationManager()->notModifiableCloudIndexRegistered(),
                                                                                                         l);
    setRowForCloudIndexRegistered(tr("Index des arêtes"), row++, l, totalMemUsed);

    setRowForCloudsRegistered(tr("Nuages synchronisés aux points"), row++, access.globalPointCloudSynchronizationManager()->cloudsRegistered(), totalMemUsed);
    setRowForCloudsRegistered(tr("Nuages synchronisés aux faces"), row++, access.globalFaceCloudSynchronizationManager()->cloudsRegistered(), totalMemUsed);
    setRowForCloudsRegistered(tr("Nuages synchronisés aux arêtes"), row++, access.globalEdgeCloudSynchronizationManager()->cloudsRegistered(), totalMemUsed);

    ui->tableWidgetMemoryUsed->setRowCount(row+1);

    QTableWidgetItem *newItem = new QTableWidgetItem(tr("Total"));
    ui->tableWidgetMemoryUsed->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem("");
    ui->tableWidgetMemoryUsed->setItem(row, 1, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2)").arg(totalMemUsed).arg(size_human(totalMemUsed)));
    ui->tableWidgetMemoryUsed->setItem(row, 2, newItem);

    for (int r = 0; r < ui->tableWidgetMemoryUsed->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidgetMemoryUsed->columnCount(); ++c)
        {
            QTableWidgetItem *item = ui->tableWidgetMemoryUsed->item(r, c);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
    }
}

void GAboutMemory::setRowForCloud(QString type, int row, CT_AbstractCloud *cloud, size_t &totalUsed)
{
    size_t memUsed = cloud->memoryUsed();

    ui->tableWidgetMemoryUsed->setRowCount(row+1);

    QTableWidgetItem *newItem = new QTableWidgetItem(type);
    ui->tableWidgetMemoryUsed->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem(tr("%1").arg(cloud->size()));
    ui->tableWidgetMemoryUsed->setItem(row, 1, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2)").arg(memUsed).arg(size_human(memUsed)));
    ui->tableWidgetMemoryUsed->setItem(row, 2, newItem);

    totalUsed += memUsed;
}

void GAboutMemory::setRowForCloudIndexRegistered(QString type, int row, QList<CT_AbstractCloudIndexRegistered *> cloudIndex, size_t &totalUsed)
{
    int nb = 0;
    size_t size = 0;
    size_t memUsed = 0;

    QListIterator<CT_AbstractCloudIndexRegistered*> it(cloudIndex);

    while(it.hasNext()) {
        CT_AbstractCloudIndexRegistered *cir = it.next();
        size += cir->abstractCloudIndex()->size();
        memUsed += cir->abstractCloudIndex()->memoryUsed();
        ++nb;
    }

    ui->tableWidgetMemoryUsed->setRowCount(row+1);

    QTableWidgetItem *newItem = new QTableWidgetItem(type);
    ui->tableWidgetMemoryUsed->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2 éléments)").arg(size).arg(nb));
    ui->tableWidgetMemoryUsed->setItem(row, 1, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2)").arg(memUsed).arg(size_human(memUsed)));
    ui->tableWidgetMemoryUsed->setItem(row, 2, newItem);

    totalUsed += memUsed;
}

void GAboutMemory::setRowForCloudsRegistered(QString type, int row, QList<CT_AbstractCloudRegistered *> clouds, size_t &totalUsed)
{
    int nb = 0;
    size_t size = 0;
    size_t memUsed = 0;

    QListIterator<CT_AbstractCloudRegistered*> it(clouds);

    while(it.hasNext()) {
        CT_AbstractCloudRegistered *cr = it.next();
        size += cr->abstractCloud()->size();
        memUsed += cr->abstractCloud()->memoryUsed();
        ++nb;
    }

    ui->tableWidgetMemoryUsed->setRowCount(row+1);

    QTableWidgetItem *newItem = new QTableWidgetItem(type);
    ui->tableWidgetMemoryUsed->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2 éléments)").arg(size).arg(nb));
    ui->tableWidgetMemoryUsed->setItem(row, 1, newItem);
    newItem = new QTableWidgetItem(tr("%1 (%2)").arg(memUsed).arg(size_human(memUsed)));
    ui->tableWidgetMemoryUsed->setItem(row, 2, newItem);

    totalUsed += memUsed;
}

void GAboutMemory::showEvent(QShowEvent *e)
{
    updateUI();

    QWidget::showEvent(e);
}
