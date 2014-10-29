#include "cloudtest.h"

#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/abstract/ct_abstractmodifiableindexcloudcolormap.h"

CloudTest::CloudTest()
{
}

void CloudTest::testPointCloudSimple()
{
    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(10, 0);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)10);

    checkPointCloud(pcir, 0);
    checkPointCloudIndex(pcir, 0);

    pcir.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);
}

void CloudTest::testPointCloudRemoveMiddle()
{
    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(10, 0);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)10);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir2 = createPointCloud(20, 52);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)30);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir3 = createPointCloud(5, 128);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)35);

    checkPointCloud(pcir, 0);
    checkPointCloud(pcir2, 52);
    checkPointCloud(pcir3, 128);

    checkPointCloudIndex(pcir, 0);
    checkPointCloudIndex(pcir2, 10);
    checkPointCloudIndex(pcir3, 30);

    pcir2.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)15);

    checkPointCloud(pcir, 0);
    checkPointCloud(pcir3, 128);

    checkPointCloudIndex(pcir, 0);
    checkPointCloudIndex(pcir3, 10);

    pcir.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)5);

    checkPointCloud(pcir3, 128);
    checkPointCloudIndex(pcir3, 0);

    pcir3.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);
}

void CloudTest::testUndefinedSizePointCloudSimple()
{
    CT_AbstractUndefinedSizePointCloud *pc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);

    for(int i=0; i<10; ++i)
    {
        CT_Point &p = pc->addPoint();
        p.setX(i);
        p.setY(i);
        p.setZ(i);
    }

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(pc);

    checkPointCloud(pcir, 0);
    checkPointCloudIndex(pcir, 0);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)10);
}

void CloudTest::testUndefinedSizePointCloudRemoveMiddle()
{
    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(10, 0);

    CT_AbstractUndefinedSizePointCloud *pc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)10);

    for(int i=13; i<33; ++i)
    {
        CT_Point &p = pc->addPoint();
        p.setX(i);
        p.setY(i);
        p.setZ(i);
    }

    CT_Repository::CT_AbstractNotModifiablePCIR pcir2 = PS_REPOSITORY->registerUndefinedSizePointCloud(pc);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)30);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir3 = createPointCloud(5, 85);

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)35);

    checkPointCloud(pcir, 0);
    checkPointCloud(pcir2, 13);
    checkPointCloud(pcir3, 85);

    checkPointCloudIndex(pcir, 0);
    checkPointCloudIndex(pcir2, 10);
    checkPointCloudIndex(pcir3, 30);

    pcir2.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)15);

    checkPointCloud(pcir, 0);
    checkPointCloud(pcir3, 85);

    checkPointCloudIndex(pcir, 0);
    checkPointCloudIndex(pcir3, 10);

    pcir3.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)10);

    checkPointCloud(pcir, 0);
    checkPointCloudIndex(pcir, 0);

    pcir.clear();

    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);
}

void CloudTest::testCloudIndexSyncRemoveMiddle()
{
    QCOMPARE(PS_REPOSITORY->globalPointCloud()->size(), (size_t)0);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(8, 0);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir2 = createPointCloud(26, 21);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir3 = createPointCloud(42, 58);

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

    cir->abstractModifiableCloudIndex()->addIndex(4); // pcir
    cir->abstractModifiableCloudIndex()->addIndex(5); // pcir
    cir->abstractModifiableCloudIndex()->addIndex(11); // pcir2
    cir->abstractModifiableCloudIndex()->addIndex(14); // pcir2
    cir->abstractModifiableCloudIndex()->addIndex(50); // pcir3
    cir->abstractModifiableCloudIndex()->addIndex(37); // pcir3
    cir->abstractModifiableCloudIndex()->addIndex(45); // pcir3

    QCOMPARE(cir->abstractModifiableCloudIndex()->size(), (size_t)7);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)4);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(1), (size_t)5);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(2), (size_t)11);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(3), (size_t)14);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(4), (size_t)37);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(5), (size_t)45);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(6), (size_t)50);

    pcir2.clear();

    QCOMPARE(cir->abstractModifiableCloudIndex()->size(), (size_t)5);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)4);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(1), (size_t)5);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(2), (size_t)11);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(3), (size_t)19);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(4), (size_t)24);

    pcir3.clear();

    QCOMPARE(cir->abstractModifiableCloudIndex()->size(), (size_t)2);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)4);
    QCOMPARE(cir->abstractModifiableCloudIndex()->constIndexAt(1), (size_t)5);

    pcir.clear();

    QCOMPARE(cir->abstractModifiableCloudIndex()->size(), (size_t)0);
}

void CloudTest::testColorCloudSyncRemoveMiddle()
{
    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(14, 0);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir2 = createPointCloud(80, 10);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir3 = createPointCloud(10, 62);

    CT_Repository::CT_CCR ccr = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud);

    for(size_t i=0; i<104; ++i)
    {
        CT_Color &col = ccr->abstractColorCloud()->colorAt(i);
        col.r = i;
        col.g = i;
        col.b = i;
        col.a = i;
    }

    pcir2.clear();

    QCOMPARE(ccr->abstractColorCloud()->size(), (size_t)24);

    for(size_t i=0; i<14; ++i)
    {
        CT_Color &col = ccr->abstractColorCloud()->colorAt(i);
        QCOMPARE(col.r, (quint8)i);
        QCOMPARE(col.g, (quint8)i);
        QCOMPARE(col.b, (quint8)i);
        QCOMPARE(col.a, (quint8)i);
    }

    for(size_t i=14; i<24; ++i)
    {
        CT_Color &col = ccr->abstractColorCloud()->colorAt(i);
        QCOMPARE(col.r, (quint8)(i+80));
        QCOMPARE(col.g, (quint8)(i+80));
        QCOMPARE(col.b, (quint8)(i+80));
        QCOMPARE(col.a, (quint8)(i+80));
    }
}

void CloudTest::testMapColorCloudSyncRemoveMiddle()
{
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> mccr = PS_REPOSITORY->createNewMapIndexCloudColor(CT_Repository::SyncWithPointCloud);

    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(10, 0);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir2 = createPointCloud(20, 10);
    CT_Repository::CT_AbstractNotModifiablePCIR pcir3 = createPointCloud(10, 30);

    QCOMPARE(mccr->abstractModifiableCloudIndex()->size(), (size_t)0);

    CT_Color col;
    ((CT_AbstractModifiableIndexCloudColorMap*)mccr->abstractModifiableCloudIndex())->insertIndexAndColor(0, col);
    ((CT_AbstractModifiableIndexCloudColorMap*)mccr->abstractModifiableCloudIndex())->insertIndexAndColor(35, col);
    ((CT_AbstractModifiableIndexCloudColorMap*)mccr->abstractModifiableCloudIndex())->insertIndexAndColor(22, col);
    ((CT_AbstractModifiableIndexCloudColorMap*)mccr->abstractModifiableCloudIndex())->insertIndexAndColor(14, col);
    ((CT_AbstractModifiableIndexCloudColorMap*)mccr->abstractModifiableCloudIndex())->insertIndexAndColor(3, col);

    QCOMPARE(mccr->abstractModifiableCloudIndex()->size(), (size_t)5);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)0);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(1), (size_t)3);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(2), (size_t)14);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(3), (size_t)22);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(4), (size_t)35);

    pcir2.clear();

    QCOMPARE(mccr->abstractModifiableCloudIndex()->size(), (size_t)3);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)0);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(1), (size_t)3);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(2), (size_t)15);

    pcir.clear();

    QCOMPARE(mccr->abstractModifiableCloudIndex()->size(), (size_t)1);
    QCOMPARE(mccr->abstractModifiableCloudIndex()->constIndexAt(0), (size_t)5);
}

void CloudTest::benchmarkCloudIndexLoop()
{
    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(15000, 0);
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

    QBENCHMARK {
        CT_AbstractModifiableCloudIndex *index = cir->abstractModifiableCloudIndex();

        size_t size = index->size();

        for(size_t i=0; i<size; ++i) {
            index->constIndexAt(i);
        }
    }
}

void CloudTest::benchmarkCloudIndexLoopDynamicCast()
{
    CT_Repository::CT_AbstractNotModifiablePCIR pcir = createPointCloud(15000, 0);
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

    QBENCHMARK {
        const CT_AbstractCloudIndexT<CT_Point> *index = dynamic_cast<const CT_AbstractCloudIndexT<CT_Point>*>(cir->abstractModifiableCloudIndex());

        size_t size = index->size();

        for(size_t i=0; i<size; ++i) {
            index->constIndexAt(i);
        }
    }
}

CT_Repository::CT_AbstractNotModifiablePCIR CloudTest::createPointCloud(size_t size, int initVar) const
{
    CT_Repository::CT_AbstractNotModifiablePCIR pcir = PS_REPOSITORY->createNewPointCloud(size);
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator begin = pcir->constBegin();
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator end = pcir->constEnd();

    float i = initVar;

    while(begin != end)
    {
        CT_Point &p = begin.cT();
        p.setX(i);
        p.setY(i);
        p.setZ(i);

        ++begin;
        ++i;
    }

    return pcir;
}

void CloudTest::checkPointCloud(CT_Repository::CT_AbstractNotModifiablePCIR pcir, int initVar) const
{
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator begin = pcir->constBegin();
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator end = pcir->constEnd();

    float i = initVar;

    while(begin != end)
    {
        CT_Point &p = begin.cT();
        QCOMPARE(p.getX(), i);
        QCOMPARE(p.getY(), i);
        QCOMPARE(p.getZ(), i);

        ++i;
        ++begin;
    }
}

void CloudTest::checkPointCloudIndex(CT_Repository::CT_AbstractNotModifiablePCIR pcir, int initIndex) const
{
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator begin = pcir->constBegin();
    CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point>::ConstIterator end = pcir->constEnd();

    size_t i = initIndex;

    while(begin != end)
    {
        QCOMPARE(begin.cIndex(), i);

        ++i;
        ++begin;
    }
}
