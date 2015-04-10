#ifndef CLOUDTEST_H
#define CLOUDTEST_H

#include <QtTest>
#include "ct_global/ct_context.h"

class CloudTest : public QObject
{
    Q_OBJECT

public:
    CloudTest();

private slots:
    //void initTestCase();
    void testPointCloudSimple();
    void testPointIterator();
    void testPointCloudRemoveMiddle();
    void testUndefinedSizePointCloudSimple();
    void testUndefinedSizePointCloudRemoveMiddle();
    void testCloudIndexSyncRemoveMiddle();
    void testColorCloudSyncRemoveMiddle();
    void testMapColorCloudSyncRemoveMiddle();
    void benchmarkCloudIndexLoop();
    void benchmarkCloudIndexLoopDynamicCast();
    //void cleanupTestCase();

private:
    CT_NMPCIR createPointCloud(size_t size, int initVar) const;
    void checkPointCloud(CT_NMPCIR pcir, int initVar, size_t size) const;
    void checkPointCloudIndex(CT_NMPCIR pcir, int initIndex, size_t size) const;
};

#endif // CLOUDTEST_H
