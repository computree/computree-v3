#ifndef STEPTEST_H
#define STEPTEST_H

#include <QObject>

class StepTest : public QObject
{
    Q_OBJECT

public:
    StepTest();

private Q_SLOTS:
    //void initTestCase();
    void testStep();
    //void cleanupTestCase();
};

#endif // STEPTEST_H
