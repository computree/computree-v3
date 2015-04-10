#include "cloudtest.h"

#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/abstract/ct_abstractmodifiableindexcloudcolormap.h"

#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_mutablepointiterator.h"
#include "ct_accessor/ct_pointaccessor.h"

CloudTest::CloudTest()
{
}

void CloudTest::testPointCloudSimple()
{
    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    CT_NMPCIR pcir = createPointCloud(10, 0);

    QCOMPARE(pAccess.size(), (size_t)10);

    checkPointCloud(pcir, 0, 10);
    checkPointCloudIndex(pcir, 0, 10);

    pcir.clear();

    QCOMPARE(pAccess.size(), (size_t)0);
}

void CloudTest::testPointIterator()
{
    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    CT_NMPCIR pcir = createPointCloud(1, 0);

    QCOMPARE(pAccess.size(), (size_t)1);

    checkPointCloud(pcir, 0, 1);
    checkPointCloudIndex(pcir, 0, 1);

    pcir.clear();

    QCOMPARE(pAccess.size(), (size_t)0);

    checkPointCloud(pcir, 0, 0);
    checkPointCloudIndex(pcir, 0, 0);
}

void CloudTest::testPointCloudRemoveMiddle()
{
    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    CT_NMPCIR pcir = createPointCloud(10, 0);

    QCOMPARE(pAccess.size(), (size_t)10);

    CT_NMPCIR pcir2 = createPointCloud(20, 52);

    QCOMPARE(pAccess.size(), (size_t)30);

    CT_NMPCIR pcir3 = createPointCloud(5, 128);

    QCOMPARE(pAccess.size(), (size_t)35);

    checkPointCloud(pcir, 0, 10);
    checkPointCloud(pcir2, 52, 20);
    checkPointCloud(pcir3, 128, 5);

    checkPointCloudIndex(pcir, 0, 10);
    checkPointCloudIndex(pcir2, 10, 20);
    checkPointCloudIndex(pcir3, 30, 5);

    pcir2.clear();

    QCOMPARE(pAccess.size(), (size_t)15);

    checkPointCloud(pcir, 0, 10);
    checkPointCloud(pcir3, 128, 5);

    checkPointCloudIndex(pcir, 0, 10);
    checkPointCloudIndex(pcir3, 10, 5);

    pcir.clear();

    QCOMPARE(pAccess.size(), (size_t)5);

    checkPointCloud(pcir3, 128, 5);
    checkPointCloudIndex(pcir3, 0, 5);

    pcir3.clear();

    QCOMPARE(pAccess.size(), (size_t)0);
}

void CloudTest::testUndefinedSizePointCloudSimple()
{
    CT_AbstractUndefinedSizePointCloud *pc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    for(int i=0; i<10; ++i)
    {
        CT_Point p;
        p.setX(i);
        p.setY(i);
        p.setZ(i);

        pc->addPoint(p);
    }

    CT_NMPCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(pc);

    checkPointCloud(pcir, 0, 10);
    checkPointCloudIndex(pcir, 0, 10);

    QCOMPARE(pAccess.size(), (size_t)10);
}

void CloudTest::testUndefinedSizePointCloudRemoveMiddle()
{
    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    CT_NMPCIR pcir = createPointCloud(10, 0);

    CT_AbstractUndefinedSizePointCloud *pc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    QCOMPARE(pAccess.size(), (size_t)10);

    for(int i=13; i<33; ++i)
    {
        CT_Point p;
        p.setX(i);
        p.setY(i);
        p.setZ(i);

        pc->addPoint(p);
    }

    CT_NMPCIR pcir2 = PS_REPOSITORY->registerUndefinedSizePointCloud(pc);

    QCOMPARE(pAccess.size(), (size_t)30);

    CT_NMPCIR pcir3 = createPointCloud(5, 85);

    QCOMPARE(pAccess.size(), (size_t)35);

    checkPointCloud(pcir, 0, 10);
    checkPointCloud(pcir2, 13, 20);
    checkPointCloud(pcir3, 85, 5);

    checkPointCloudIndex(pcir, 0, 10);
    checkPointCloudIndex(pcir2, 10, 20);
    checkPointCloudIndex(pcir3, 30, 5);

    pcir2.clear();

    QCOMPARE(pAccess.size(), (size_t)15);

    checkPointCloud(pcir, 0, 10);
    checkPointCloud(pcir3, 85, 5);

    checkPointCloudIndex(pcir, 0, 10);
    checkPointCloudIndex(pcir3, 10, 5);

    pcir3.clear();

    QCOMPARE(pAccess.size(), (size_t)10);

    checkPointCloud(pcir, 0, 10);
    checkPointCloudIndex(pcir, 0, 10);

    pcir.clear();

    QCOMPARE(pAccess.size(), (size_t)0);
}

void CloudTest::testCloudIndexSyncRemoveMiddle()
{
    CT_PointAccessor pAccess;
    QCOMPARE(pAccess.size(), (size_t)0);

    CT_NMPCIR pcir = createPointCloud(8, 0);
    CT_NMPCIR pcir2 = createPointCloud(26, 21);
    CT_NMPCIR pcir3 = createPointCloud(42, 58);

    CT_MCIR cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

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
    CT_NMPCIR pcir = createPointCloud(14, 0);
    CT_NMPCIR pcir2 = createPointCloud(80, 10);
    CT_NMPCIR pcir3 = createPointCloud(10, 62);

    CT_CCR ccr = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud);

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
    CT_MCIR mccr = PS_REPOSITORY->createNewMapIndexCloudColor(CT_Repository::SyncWithPointCloud);

    CT_NMPCIR pcir = createPointCloud(10, 0);
    CT_NMPCIR pcir2 = createPointCloud(20, 10);
    CT_NMPCIR pcir3 = createPointCloud(10, 30);

    QCOMPARE(mccr->abstractModifiableCloudIndex()->size(), (size_t)0);

    CT_Color col;
    CT_AbstractModifiableIndexCloudColorMap *map = dynamic_cast<CT_AbstractModifiableIndexCloudColorMap*>(mccr->abstractModifiableCloudIndex());
    map->insertIndexAndColor(0, col);
    map->insertIndexAndColor(35, col);
    map->insertIndexAndColor(22, col);
    map->insertIndexAndColor(14, col);
    map->insertIndexAndColor(3, col);

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
    CT_NMPCIR pcir = createPointCloud(15000, 0);
    CT_MCIR cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

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
    CT_NMPCIR pcir = createPointCloud(15000, 0);
    CT_MCIR cir = PS_REPOSITORY->createNewIndexCloud(CT_Repository::SyncWithPointCloud);

    QBENCHMARK {
        const CT_AbstractPointCloudIndex *index = dynamic_cast<const CT_AbstractPointCloudIndex*>(cir->abstractModifiableCloudIndex());

        size_t size = index->size();

        for(size_t i=0; i<size; ++i) {
            index->constIndexAt(i);
        }
    }
}

CT_NMPCIR CloudTest::createPointCloud(size_t size, int initVar) const
{
    CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(size);

    CT_MutablePointIterator it(pcir);

    double i = initVar;

    while(it.hasNext())
    {
        CT_Point p;
        p.setX(i);
        p.setY(i);
        p.setZ(i);

        it.next().replaceCurrentPoint(p);
        ++i;
    }

    return pcir;
}

void CloudTest::checkPointCloud(CT_NMPCIR pcir, int initVar, size_t size) const
{
    size_t n = 0;

    CT_PointIterator it(pcir);

    double i = initVar;

    while(it.hasNext())
    {
        const CT_Point &p = it.next().cT();
        QCOMPARE(p(CT_Point::X), i);
        QCOMPARE(p(CT_Point::Y), i);
        QCOMPARE(p(CT_Point::Z), i);

        ++i;
        ++n;
    }

    QCOMPARE(n, size);
}

void CloudTest::checkPointCloudIndex(CT_NMPCIR pcir, int initIndex, size_t size) const
{
    size_t n = 0;

    CT_PointIterator it(pcir);

    size_t i = initIndex;

    while(it.hasNext())
    {
        QCOMPARE(it.next().cIndex(), i);

        ++i;
        ++n;
    }

    QCOMPARE(n, size);
}
